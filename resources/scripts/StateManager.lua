StateManager = {
    getStateMachine = function(e)
        local handle = Handles[e:getId()]
        if not handle.components.stateMachine then return end
        return handle.components.stateMachine
    end,

    changeState = function(e, stateName)
        local sm = StateManager.getStateMachine(e)
        if sm:getStateName() == stateName then
            return
        end
        if sm:getStateName() then
            local s = sm:getState()
            if s.exit then
                s.exit(e)
            end
        end
        sm:setStateName(stateName)
        local s = sm:getState()
        if s.init then
            s.init(e)
        end
    end,

    initState = function(e)
        local sm = StateManager.getStateMachine(e)
        StateManager.changeState(e, sm:get().initialState)
        sm:setStateName(sm:get().initialState)
    end,

    update = function(e)
        local sm = StateManager.getStateMachine(e)
        if not sm then return end
        local state = sm:getState()
        if state and state.update then
            local nextState = state.update(e)
            if nextState then
                StateManager.changeState(e, nextState)
            end
        end
    end,

    onEvent = function(e, event)
        local sm = StateManager.getStateMachine(e)
        if not sm then return end
        local callback = sm:getState()[event.type]
        if callback then
            print('Call '..sm:getStateName()..'.'..event.type..' with '..e:getName())
            local nextState = callback(e, event)
            if nextState then
                StateManager.changeState(e, nextState)
            end
        end
    end,

    onKeyUp = function(e, code)
        local sm = StateManager.getStateMachine(e)
        local callback = sm:getState()["onKeyUp"]
        if callback then
            local nextState = callback(e, code)
            if nextState then
                StateManager.changeState(e, nextState)
            end
        end
    end,

    onKeyDown = function(e, code)
        local sm = StateManager.getStateMachine(e)
        local callback = sm:getState()["onKeyDown"]
        if callback then
            local nextState = callback(e, code)
            if nextState then
                StateManager.changeState(e, nextState)
            end
        end
    end,
}

return StateManager