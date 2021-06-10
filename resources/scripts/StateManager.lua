StateManager = {
    currentState = nil,

    changeState = function(entity, state)
        local sm = getComponent(entity, "StateMachine")
        if sm.currentState==state then
            return
        end
        if sm.currentState then
            local state = sm.states[sm.currentState]
            if state.exit then
                state.exit(entity)
            end
        end
        sm.currentState = state
        local state = sm.states[sm.currentState]
        if state.init then
            state.init(entity)
        end
    end,

    initState = function(entity)
        local sm = getComponent(entity, "StateMachine")
        StateManager.changeState(entity, sm.initialState)
        sm.currentState = sm.initialState
    end,

    update = function(entity)
        local sm = getComponent(entity, "StateMachine")
        local state = sm.states[sm.currentState]
        if state.update then
            local nextState = state.update(entity)
            if nextState then
                StateManager.changeState(entity, nextState)
            end
        end
    end,

    onEvent = function(entity, event)
        local sm = getComponent(entity, "StateMachine")
        if not sm then
            return nil
        end
        local callback = sm.states[sm.currentState][event.type]
        if callback then
            local nextState = callback(entity, event)
            if nextState then
                StateManager.changeState(entity, nextState)
            end
        end
    end,

    onKeyUp = function(entity, code)
        local sm = getComponent(entity, "StateMachine")
        local callback = sm.states[sm.currentState]["onKeyUp"]
        if callback then
            local nextState = callback(entity, code)
            if nextState then
                StateManager.changeState(entity, nextState)
            end
        end
    end,

    onKeyDown = function(entity, code)
        local sm = getComponent(entity, "StateMachine")
        local callback = sm.states[sm.currentState]["onKeyDown"]
        if callback then
            local nextState = callback(entity, code)
            if nextState then
                StateManager.changeState(entity, nextState)
            end
        end
    end,
}

return StateManager