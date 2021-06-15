local Component = require 'Component'
local TimerComponent = Component:subclass('TimerComponent')

TimerComponent.static.name = 'timer'

function TimerComponent:initialize()
    Component.initialize(self)
    self.time = 0
end

function TimerComponent:reset()
    self.time = 0
end

function TimerComponent:get()
    return self.time
end

function TimerComponent:update()
    self.time = self.time + 1
end

return TimerComponent
