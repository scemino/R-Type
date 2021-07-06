local Action = require 'actions.Action'
local TweenAction = Action:subclass("TweenAction")

local function linear(t)
    return t
end

local function easeIn(t)
    return t * t * t * t
end

local function easeOut(t)
    local f = (t - 1)
    return f * f * f * (1 - t) + 1
end

function TweenAction:initialize(params)
    Action.initialize(self)

    -- if params = { p = ... }, then we do self.p = params.p
    local tableMerge = require 'util.table'.tableMerge
    tableMerge(self, params)

    self.distance = self.endValue - self.startValue

    self.f = linear
    if params.type then
        if params.type == "linear" then
            self.f = linear
        elseif params.type == "easeIn" then
            self.f = easeIn
        elseif params.type == "easeOut" then
            self.f = easeOut
        else
            error(string.format("Unsupported tween easing function type '%s'", params.type))
        end
    end
end

function TweenAction:enter()
    self.currentTime = 0
    self.currentValue = self.startValue

    self:setValue(self.startValue)
end

function TweenAction:execute(dt)
    self.currentTime = self.currentTime + dt
    -- don't want to overshoot! Check value before setting it
    if self.currentTime >= self.duration then
        self.finished = true
        return
    end

    self.currentValue = self.startValue + self.distance * self.f(self.currentTime / self.duration)
    self:setValue(self.currentValue)
end

function TweenAction:exit()
    self:setValue(self.endValue)
end

function TweenAction:setValue(value)
    self.update(self.caller, value, self)
end

return TweenAction
