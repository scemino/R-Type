local EnemyStateMachine = {
    states = {
        MoveState = {
            update = function(e)
                e.components.enemyPosition:updatePosition(e)
            end,
            hit = function(_, event)
                if event.data.collisionType == 'tile' then
                    return 'ExplodingState'
                end
            end
        },
        ExplodingState = {
            init = function(e)
                print('BOOM', e)
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