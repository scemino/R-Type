local function shoot(pos)
    if not hasEntity('player') then
        return
    end

    -- probability to shoot
    if math.random() > 0.994 then
        EntityFactory.enemyShoot(pos)
    end
end

local function bulletHitEnemy(bullet, enemy)
    local damage = bullet.components.damage:getDamage()
    bullet.components.damage:removeDamage(enemy.components.health:getHealth())
    enemy.components.health:setDamage(damage)
end

local EnemyStateMachine = {
    states = {
        MoveState = {
            update = function(e)
                if e.components.health:isAlive() then
                    e.components.enemyPosition:updatePosition(e)
                    shoot(e:getPosition())
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
return EnemyStateMachine