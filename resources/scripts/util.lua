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

function util.distance(p1, p2)
    local x = p1.x - p2.x
    local y = p1.y - p2.y
    return math.sqrt(x * x + y * y)
end

return util