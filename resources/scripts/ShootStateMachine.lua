local ShootStateMachine = {
    states = {
        MoveState = {
            hit = function(e, event)
                if event.data.collisionType == 'tile' then
                    e:setVelocity(vec(0,0))
                    return 'ExplodingState'
                else
                    e:die()
                end
            end
        },
        ExplodingState = {
            init = function(e)
                playSound(Sounds.shoot_explode2)
                e:setAnim('shoot_explode', 1)
            end,
            anim = function(e, event)
                if event.data.name == 'shoot_explode' then
                    e:die()
                end
            end
        }
    },
    initialState = 'MoveState'
}
return ShootStateMachine