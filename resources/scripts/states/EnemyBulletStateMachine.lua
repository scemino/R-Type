local EnemyBulletStateMachine = {
    states = {
        MoveState = {
            update = function(e)
            end,
            hit = function(e, event)
                if event.data.collisionType == 'entities' then
                    if event.data.entity:getName() == 'player' then
                        return 'ExplodingState'
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
                e:setVelocity(vec(-1,0))
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