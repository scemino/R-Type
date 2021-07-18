local function forceHitBullet(force, bullet)
    local damage = force.components.damage:getDamage()
    force.components.damage:removeDamage(bullet.components.health:getHealth())
    bullet.components.health:setDamage(damage)
end

local EnemyBulletStateMachine = {
    states = {
        MoveState = {
            update = function(e)
                if not e.components.health:isAlive() then
                    return 'ExplodingState'
                end
            end,
            hit = function(e, event)
                if event.data.collisionType == 'entities' then
                    if event.data.entity:getName() == 'force' then
                        forceHitBullet(event.data.entity, e)
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
                -- wow I need to change this (we shouldn't know the camera velocity here)
                e:setVelocity(vec(-getEntity('camera'):getVelocity().x, 0))
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
return EnemyBulletStateMachine