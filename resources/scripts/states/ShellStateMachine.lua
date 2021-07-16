local ShellStateMachine = {
    states = {
        MoveState = {
            update = function(e)
                e.components.shell:update()
            end
        }
    },
    initialState = 'MoveState'
}
return ShellStateMachine