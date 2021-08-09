local TabrokStateMachine = {
    states = {
        FallingState = {
            init = function(e)
                e:setAnim('flying', -1)
                e:setFlipX(true)
                e:setVelocity(vec(0, 1))
            end,
            update = function(e)
                local nextPos = getEntity('camera'):getPosition() + e:getPosition() + e:getVelocity()
                local tile = map(nextPos)
                if tile ~= -1 then
                    e:setVelocity(vec(0, 0))
                    return 'LandingState'
                end
            end
        },
        LandingState = {
            init = function(e)
                e:setAnim('landing', 1)
            end,
            anim = function(_, _)
                return 'Waiting1State'
            end,
        },
        Waiting1State = {
            init = function(e)
                e:setAnim('waiting', 1)
                e:setVelocity(vec(0, 0))
            end,
            anim = function(_, _)
                return 'WalkingState'
            end,
        },
        WalkingState = {
            init = function(e)
                e:setAnim('walking', -1)
                e:setVelocity(vec(-0.8, 0))
            end,
            update = function(e)
                local flipX = e:getFlipX()
                -- check if the enemy will hit a tile next update
                -- this offset is relative to sprite origin (bottom center)
                local offset = vec(24, -4)
                if flipX then
                    offset.x = -24
                end
                local nextPos = getEntity('camera'):getPosition() + e:getPosition() + e:getVelocity() + offset
                local tile = map(nextPos)
                if tile ~= -1 then
                   return 'Waiting2State'
                end
            end,
        },
        Waiting2State = {
            init = function(e)
                e:setAnim('waiting', 1)
                e:setVelocity(vec(0, 0))
            end,
            anim = function(_, _)
                return 'FlyingState'
            end,
        },
        FlyingState = {
            init = function(e)
                e:setAnim('flying', 7)
                e:setFlipX(true)
                e:setVelocity(vec(0.5, -1))
            end,
            anim = function(e, _)
                return 'FlyingHState'
            end,
        },
        FlyingHState = {
            init = function(e)
                e:setAnim('flying', 2)
                e:setFlipX(true)
                e:setVelocity(vec(1, 0))
            end,
            anim = function(e, _)
                return 'FlyingHFastState'
            end,
        },
        FlyingHFastState = {
            init = function(e)
                e:setAnim('flying', 4)
                e:setFlipX(true)
                e:setVelocity(vec(2, 0))
            end,
            anim = function(_, _)
                return 'FlyingH2State'
            end,
        },
        FlyingH2State = {
            init = function(e)
                e:setAnim('flying', 5)
                e:setFlipX(true)
                e:setVelocity(vec(1, 0))
            end,
            anim = function(_, _)
                return 'Falling2State'
            end,
        },
        Falling2State = {
            init = function(e)
                e:setAnim('flying', -1)
                e:setFlipX(true)
                e:setVelocity(vec(0.5, 1))
            end,
            update = function(e)
                local nextPos = getEntity('camera'):getPosition() + e:getPosition() + e:getVelocity()
                local tile = map(nextPos)
                if tile ~= -1 then
                    e:setVelocity(vec(0, 0))
                    return 'Landing2State'
                end
            end
        },
        Landing2State = {
            init = function(e)
                e:setAnim('landing', 1)
            end,
            anim = function(_, _)
                return 'StandingState'
            end,
        },
        StandingState = {

        }
    },
    initialState = 'FallingState'
}
return TabrokStateMachine