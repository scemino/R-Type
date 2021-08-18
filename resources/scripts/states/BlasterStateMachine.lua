local util = require 'util'

local function shoot(pos)
    if not hasEntity('player') then
        return
    end

    -- probability to shoot
    if math.random() > 0.994 then
        EntityFactory.enemyShoot(pos + vec(0, -8))
    end
end

local function bulletHitEnemy(bullet, enemy)
    local damage = bullet.components.damage:getDamage()
    bullet.components.damage:removeDamage(enemy.components.health:getHealth())
    enemy.components.health:setDamage(damage)
end

local function getFrameFromAngle(angle)
    if angle < 22.5 then
        return 5
    end
    if angle < 67.5 then
        return 4
    end
    if angle <= 90 then
        return 3
    end
    if angle < 112.5 then
        return 2
    end
    if angle < 157.5 then
        return 1
    end
    return 0
end

local BlasterStateMachine = {
    states = {
        MoveState = {
            update = function(e)
                if e.components.health:isAlive() then
                    if not hasEntity('player') then
                        return
                    end
                    local player = getEntity('player')
                    local playerPos = player:getPosition()
                    local pos = e:getPosition()
                    local d = playerPos - pos
                    local angle = math.atan(-d.y, d.x)
                    if angle < 0 or angle > 180 then
                        angle = -angle
                    end
                    e:setFrame(getFrameFromAngle(angle * 180.0 / math.pi))
                    shoot(e:getPosition())
                    util.checkBounds(e)
                else
                    return 'ExplodingState'
                end
            end,
            hit = function(e, event)
                if event.data.collisionType == 'entities' then
                    local name = event.data.entity:getName()
                    if name == 'player' then
                        return 'ExplodingState'
                    elseif name == 'shoot' or name == 'force' or name == 'bits_up' or name == 'bits_dn' then
                        bulletHitEnemy(event.data.entity, e)
                    end
                end
            end
        },
        ExplodingState = {
            init = function(e)
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
    initialState = 'MoveState'
}
return BlasterStateMachine