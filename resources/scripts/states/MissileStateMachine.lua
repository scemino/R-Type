local function bulletHit(bullet, enemy)
    local damage = bullet.components.damage:getDamage()
    bullet.components.damage:removeDamage(enemy.components.health:getHealth())
    enemy.components.health:setDamage(damage)
end

local MissileStateMachine = {
    states = {
        MoveState = {
            update = function(e)
                if not e.components.health:isAlive() then
                    return 'ExplodingState'
                end
                e.components.missile:update()
            end,
            hit = function(e, event)
                if event.data.collisionType == 'entities' then
                    local name = event.data.entity:getName()
                    if name == 'shoot' or name == 'force' or name == 'bits_up' or name == 'bits_dn' then
                        bulletHit(event.data.entity, e)
                    end
                elseif event.data.collisionType == 'screen' then
                    e:die()
                elseif event.data.collisionType == 'tile' then
                    return 'ExplodingState'
                end
            end
        },
        ExplodingState = {
            init = function(e)
                e:setAnim('missile_exploding', 1)
                e:setFlipY(false)
            end,
            anim = function(e, event)
                if event.data.name == 'missile_exploding' then
                    e:die()
                end
            end
        }
    },
    initialState = 'MoveState'
}
return MissileStateMachine