local Action = require 'actions.Action'

local DelayAction = Action:subclass("DelayAction")

function DelayAction:initialize(time)
    Action.initialize(self)
    self.time = time
end

function DelayAction:enter()
    self.currentTime = 0
end

function DelayAction:execute(dt)
    self.currentTime = self.currentTime + dt
    if self.currentTime >= self.time then
        self.finished = true
    end
end

return DelayAction