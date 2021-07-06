local class = require 'lib.middleclass'
local Action = class("Action")

function Action:initialize()
    self.finished = false
end

function Action:run()
    self:enter()
    while not self.finished do
        coroutine.yield()

        -- action finished between two coroutine updates (e.g. by reacting to event)
        if self.finished then
            break
        end

        self:execute(1.0 / 60.0)
    end

    self:exit()

    return self.actionReturn
end

function Action:enter()
end

function Action:execute(dt)
end

function Action:exit()
end

function Action:isFinished()
    return self.finished
end

return Action