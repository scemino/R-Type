local function bulletHitEnemy(bullet, enemy)
    local damage = bullet.components.damage:getDamage()
    bullet.components.damage:removeDamage(enemy.components.health:getHealth())
    enemy.components.health:setDamage(damage)
end

local function hitEntities(e, event)
    if event.data.collisionType == 'entities' then
        local name = event.data.entity:getName()
        if name == 'shoot' or name == 'force' or name == 'bits_up' or name == 'bits_dn' then
            bulletHitEnemy(event.data.entity, e)
            if not e.components.health:isAlive() then
                return 'ExplodingState'
            end
        end
    end
end

local CancerStateMachine = {
    states = {
        MovingState = {
            init = function(e)
                e:setAnim('default', -1)
                e:setVelocity(vec(-1, 1))
            end,
            hit = function(e, event)
                if event.data.collisionType == 'tile' then
                    -- choose direction to player
                    if not hasEntity('player') then
                        return
                    end
                    local playerPos = getEntity('player'):getPosition()
                    local cancerPos = e:getPosition()
                    local dir = playerPos - cancerPos
                    local vel = vec(-1, 1)
                    if dir.x > 0 then
                        vel.x = 1
                    end
                    if dir.y < 0 then
                        vel.y = -1
                    end
                    e:setVelocity(vel * (0.1 + 0.5 * math.random()))
                end
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
return CancerStateMachine