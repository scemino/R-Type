local HealthComponent = require 'components.HealthComponent'
local DamageComponent = require 'components.DamageComponent'
local MissileComponent = require 'components.MissileComponent'
local StateMachineComponent = require 'components.StateMachineComponent'

local function shoot(leftDir, pos)
    local e = Entity()
    e:emplace('Name', { name = 'enemy_missile' })
    e:emplace('Position')
    e:emplace('Graphics')
    e:emplace('Collide', { size = vec(16, 16) })
    e:emplace('Animation', { name = 'resources/anims/p_staff.json' })
    e:setPosition(pos)
    e:setAnim('missile', 0)
    e:setFlipX(leftDir)
    addComponent(e, HealthComponent(1))
    addComponent(e, DamageComponent(1))
    addComponent(e, MissileComponent(leftDir, pos + vec(20, -25)))
    addComponent(e, StateMachineComponent('states.MissileStateMachine'))
    StateManager.initState(e)
end

local function bulletHit(bullet, enemy)
    local damage = bullet.components.damage:getDamage()
    bullet.components.damage:removeDamage(enemy.components.health:getHealth())
    enemy.components.health:setDamage(damage)
end

local function hitEntities(e, event)
    if event.data.collisionType == 'entities' then
        local name = event.data.entity:getName()
        if name == 'player' then
            return 'ExplodingState'
        elseif name == 'shoot' or name == 'force' or name == 'bits_up' or name == 'bits_dn' then
            bulletHit(event.data.entity, e)
            if not e.components.health:isAlive() then
                return 'ExplodingState'
            end
        end
    end
end

local PStaffStateMachine = {
    states = {
        WalkingState = {
            init = function(e)
                e:setAnim('walking', -1)
                e:setFlipX(true)
                e:setVelocity(vec(-1, 0))
            end,
            update = function(e)
                local flipX = e:getFlipX()
                -- check if the enemy will hit a tile next update
                -- this offset is relative to sprite origin (bottom left)
                local offset = vec(33, -8)
                if flipX then
                    offset.x = 0
                end
                local nextPos = getEntity('camera'):getPosition() + e:getPosition() + e:getVelocity() + offset
                local tile = map(nextPos)
                if tile ~= -1 then
                    -- yes so change direction
                    e:setFlipX(not flipX)
                    local vel = e:getVelocity()
                    e:setVelocity(vec(-vel.x, 0))
                end

                -- early exit
                if not hasEntity('player') then
                    return
                end

                local player = getEntity('player')
                local targetX = player:getPosition().x
                local d = math.abs(e:getPosition().x - targetX)
                if d < 60 then
                    return 'AimingState'
                end
            end,
            hit = function(e, event)
                return hitEntities(e, event)
            end
        },
        AimingState = {
            init = function(e)
                e:setVelocity(vec(0, 0))
                e:setAnim('aiming', 1)
                if e.components.pstaff:getCount() == 1 then
                    e.components.pstaff:setLeftDir(e:getFlipX())
                end
            end,
            anim = function(_, event)
                if event.data.name == 'aiming' then
                    return 'ShootingState'
                end
            end,
            hit = function(e, event)
                return hitEntities(e, event)
            end
        },
        ShootingState = {
            init = function(e)
                e:setAnim('shooting', 1)
                shoot(e.components.pstaff:getLeftDir(), e:getPosition())
            end,
            anim = function(_, event)
                if event.data.name == 'shooting' then
                    return 'WaitingState'
                end
            end,
            hit = function(e, event)
                return hitEntities(e, event)
            end
        },
        WaitingState = {
            init = function(e)
                e:setAnim('waiting', 1)
            end,
            anim = function(e, event)
                if event.data.name == 'waiting' then
                    if e.components.pstaff:getCount() < 7 then
                        e.components.pstaff:increment()
                        return 'ShootingState'
                    else
                        e.components.pstaff:reset()
                        return 'NoAimingState'
                    end
                end
            end,
            hit = function(e, event)
                return hitEntities(e, event)
            end
        },
        NoAimingState = {
            init = function(e)
                e:setAnim('no_aiming', 1)
            end,
            anim = function(_, event)
                if event.data.name == 'no_aiming' then
                    return 'WalkingState'
                end
            end,
            hit = function(e, event)
                return hitEntities(e, event)
            end
        },
        ExplodingState = {
            init = function(e)
                e:setVelocity(vec(0, 0))
                playSound(Sounds.enemy1_explode)
                e:setAnim('exploding', 1)
            end,
            anim = function(e, event)
                if event.data.name == 'exploding' then
                    e:die()
                end
            end
        }
    },
    initialState = 'WalkingState'
}
return PStaffStateMachine