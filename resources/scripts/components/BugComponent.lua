local Component = require 'Component'
local BugComponent = Component:subclass('BugComponent')

BugComponent.static.name = 'bug'

function BugComponent:initialize(pos, index)
    Component.initialize(self)
    self.initPosY = pos.y
    self.index = index
    self.angle = index * 60
    self.pos = pos + vec(index * 30, self:getY())
end

function BugComponent:setEntity(entity)
    self.entity = entity
    self.entity:setPosition(self.pos)
end

function BugComponent:getFrameFromAngle(angle)
    angle = angle + 270
    if angle > 360 then
        angle = angle - 360
    end
    if angle >= 0 and angle < 22 then
        return 8
    elseif angle < 66 then
        return 7
    elseif angle < 110 then
        return 6
    elseif angle < 150 then
        return 7
    elseif angle < 194 then
        return 8
    elseif angle < 238 then
        return 9
    elseif angle < 282 then
        return 10
    elseif angle < 324 then
        return 9
    elseif angle < 360 then
        return 8
    end
end

function BugComponent:getY()
    local sina = math.sin(self.angle / math.pi)
    local y = math.floor(20 * sina)
    return self.initPosY + y
end

function BugComponent:updatePosition()
    local x = self.entity:getPosition().x

    self.angle = x / 10.0
    if self.angle < 0 then
        self.angle = self.angle + 360
    end
    --print('angle=', self.angle)

    -- angle with floor
    local cosa = math.cos(self.angle / math.pi)
    local sina = math.sin(self.angle / math.pi)
    local n = vec(cosa, sina)
    local angle = 180.0 * math.atan(n.y, n.x) / math.pi
    if angle < 0 then
        angle = angle + 360
    end

    local y = math.floor(20 * sina)
    y = self.initPosY + y
    self.entity:setPosition(vec(x, y))
    self.entity:setFrame(self:getFrameFromAngle(angle))
end

return BugComponent
