local ShootStateMachine = {
    states = {
        MoveState = {
            update = function(e)
                if e.components.damage:getDamage() == 0 then
                    return 'ExplodingState'
                end
            end,
            hit = function(e, event)
                if event.data.collisionType == 'screen' then
                    e:die()
                elseif event.data.collisionType == 'tile' then
                    return 'ExplodingState'
                end
            end
        },
        ExplodingState = {
            init = function(e)
                e:setVelocity(vec(-getEntity('camera'):getVelocity().x, 0))
                playSound(Sounds.shoot_explode2)
                if e.components.damage:getInitialDamage() == 20 then
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