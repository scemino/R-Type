local DelayAction = require 'actions.DelayAction'
function delay(...)
    local action = DelayAction:new(...)
    action:run()
end

local TweenAction = require 'actions.TweenAction'
local function tween(...)
    local action = TweenAction:new(...)
    action:run()
end

function fadeIn(time)
    tween {
        startValue = 1.0,
        endValue = 0.0,
        duration = time,
        caller = nil,
        update = function(_, value)
            setFade(value)
        end
    }
end

function fadeOut(time)
    tween {
        startValue = 0.0,
        endValue = 1.0,
        duration = time,
        caller = nil,
        update = function(_, value)
            setFade(value)
        end
    }
end

function moveTo(entity, pos, time, type)
    tween {
        startValue = entity:getPosition(),
        endValue = pos,
        duration = time,
        caller = entity,
        type = type,
        update = function(e, value)
            e:setPosition(value)
        end
    }
end