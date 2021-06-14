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
                if Handles[e:getId()].components.damage:getDamage() == 1 then
                    e:setAnim('shoot_explode', 1)
                else
                    e:setAnim('shoot_explode_big', 1)
                end
            end,
            anim = function(e, event)
                if event.data.name == 'shoot_explode' or event.data.name == 'shoot_explode_big' then
                    e:die()
                end
            end
        }
    },
    initialState = 'MoveState'
}
return ShootStateMachine