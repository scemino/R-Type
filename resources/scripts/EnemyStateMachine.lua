local EnemyStateMachine = {
    states = {
        MoveState = {
            update = function(e)
                e.components.enemyPosition:updatePosition(e)
            end,
            hit = function(e, event)
                if event.data.collisionType == 'entities' then
                    if event.data.entity:getName() == 'player' then
                        return 'ExplodingState'
                    elseif event.data.entity:getName() == 'shoot' then
                        -- TODO: find a way to get this handle from event
                        local handle = Handles[event.data.entity:getId()]
                        local damage = handle.components.damage:getDamage()
                        print('hit entity '..event.data.entity:getName()..' with damage '..damage)
                        handle.components.damage:removeDamage(e.components.health:getHealth())
                        e.components.health:setDamage(damage)
                        if not e.components.health:isAlive() then
                            return 'ExplodingState'
                        end
                    end
                end
            end
        },
        ExplodingState = {
            init = function(e)
                print('BOOM', e)
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