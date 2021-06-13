local EnemyStateMachine = {
    states = {
        MoveState = {
            update = function(entity)
                local handle = Handles[entity:getId()]
                handle.components.enemyPosition:updatePosition()
            end,
            exit = function(entity)
                print('MoveState exit')
            end,
            hit = function(entity, event)
                if event.data.collisionType == 'tile' then
                    return 'ExplodingState'
                end
            end
        },
        ExplodingState = {
            init = function(entity)
                print('BOOM', entity)
                entity:setAnim('explode', 1)
            end,
            anim = function(entity, event)
                if event.data.name == 'explode' then
                    entity:die()
                end
            end
        }
    },
    initialState = 'MoveState'
}
return EnemyStateMachine