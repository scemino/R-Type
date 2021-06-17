local LivesIndicatorStateMachine = {
    states = {
        InitState = {
            onKeyUp = function(e, code)
                if code == Keys.H then
                    local player = Handles[getEntity('player'):getId()]
                    player.components.lives:setLives(player.components.lives:getLives() + 1)
                elseif code == Keys.G then
                    local player = Handles[getEntity('player'):getId()]
                    player.components.lives:setLives(player.components.lives:getLives() - 1)
                end
            end
        }
    },
    initialState = 'InitState'
}
return LivesIndicatorStateMachine