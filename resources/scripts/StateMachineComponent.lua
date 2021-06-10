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

return StateMachineComponent
