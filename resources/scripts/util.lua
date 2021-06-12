local util = {
}

function util.clamp(value, min, max)
    if value < min then
        value = min
    end
    if value > max then
        value = max
    end
    return value
end

function util.clampVector(value, min, max)
    value.x = util.clamp(value.x, min.x, max.x)
    value.y = util.clamp(value.y, min.y, max.y)
    return value
end

return util