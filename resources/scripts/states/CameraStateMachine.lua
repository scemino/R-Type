local CameraStateMachine = {
    states = {
        MoveState = {
            init = function(e)
                --e:setVelocity(vec(0.5, 0))
                --e:setPosition(vec(1200, 0))
            end,
            update = function(e)
                local pos = e:getPosition();
                getEntity('level'):setPosition(vec(-pos.x, -pos.y))
                setLevelPosition(math.floor(pos.x))
            end
        }
    },
    initialState = 'MoveState'
}
return CameraStateMachine