local util = require 'util'
local HealthComponent = require 'components.HealthComponent'
local DamageComponent = require 'components.DamageComponent'
local StateMachineComponent = require 'components.StateMachineComponent'

local function shoot(pos)
    local e = Entity()
    e:emplace('Name', { name = 'enemy_bullet' })
    e:emplace('Position')
    e:emplace('Motion')
    e:emplace('Graphics')
    e:emplace('Collide', { size = vec(63, 16) })
    e:emplace('Animation', { name = 'resources/anims/scant_bullet.json' })
    e:setPosition(pos + vec(0, -4))
    e:setVelocity(vec(-4, 0))
    e:setAnim('move', -1)
    addComponent(e, HealthComponent(1))
    addComponent(e, DamageComponent(1))
    addComponent(e, StateMachineComponent('states.EnemyBulletStateMachine'))
    StateManager.initState(e)
end

local function bulletHitEnemy(bullet, enemy)
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
            bulletHitEnemy(event.data.entity, e)
            if not e.components.health:isAlive() then
                return 'ExplodingState'
            end
        end
    end
end

local ScantStateMachine = {
    states = {
        MovingState = {
            init = function(e)
                e:setAnim('default', 1)
                e:setFrame(0)
            end,
            update = function(e)
                if not hasEntity('player') then
                    return
                end
                local player = getEntity('player')
                local target = player:getPosition() + vec(80, 0)
                local scantPos = e:getPosition()
                local d = util.distance(target, scantPos)
                if d < 100 then
                    e:setVelocity(vec(0, 0))
                    return 'AttackingState'
                end
                local dx = (target.x - scantPos.x) / d
                local dy = (target.y - scantPos.y) / d
                e:setVelocity(vec(dx, dy))
                if dy > 0 then
                    e:setFrame(2)
                else
                    e:setFrame(1)
                end
            end,
            hit = function(e, event)
                return hitEntities(e, event)
            end
        },
        AttackingState = {
            init = function(e)
                e:setAnim('shoot', 1)
                shoot(e:getPosition())
                e:setVelocity(vec(1, 0))
            end,
            anim = function(_, _)
                return 'MovingState'
            end,
            hit = function(e, event)
                return hitEntities(e, event)
            end
        },
        ExplodingState = {
            init = function(e)
                e:setVelocity(vec(0, 0))
                playSound(Sounds.enemy1_explode)
                e:setAnim('explode', 1)
            end,
            anim = function(e, event)
                if event.data.name == 'explode' then
                    e:die()
                end
            end
        }
    },
    initialState = 'MovingState'
}
return ScantStateMachine