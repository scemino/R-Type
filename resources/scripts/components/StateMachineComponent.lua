local Component = require 'Component'
local StateMachineComponent = Component:subclass('StateMachineComponent')

StateMachineComponent.static.name = 'stateMachine'

function StateMachineComponent:initialize(stateMachine)
    Component.initialize(self)
    self.stateMachine = require(stateMachine)
end

function StateMachineComponent:get()
    return self.stateMachine
end

function StateMachineComponent:setStateName(stateName)
    self.stateName = stateName
end

function StateMachineComponent:getStateName()
    return self.stateName
end

function StateMachineComponent:getState()
    return self.stateMachine.states[self.stateName]
end

return StateMachineComponent
