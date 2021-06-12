local CameraStateMachine = {
    states = {
        MoveState = {
            init = function(e)
                e:setVelocity(vec(1, 0))
            end,
            update = function(e)
                local pos = e:getPosition();
                setLevelPosition(math.floor(pos.x))
            end
        }
    },
    initialState = 'MoveState'
}
return CameraStateMachine