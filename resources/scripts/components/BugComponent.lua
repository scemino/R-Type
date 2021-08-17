local Component = require 'Component'
local BugComponent = Component:subclass('BugComponent')

BugComponent.static.name = 'bug'

function BugComponent:initialize(pos, index, mode)
    Component.initialize(self)
    self.initPosY = pos.y
    self.index = index
    self.angle = index * 60
    self.pos = pos + vec(index * 30, self:getY())
    self.update = self.update1
    self.mode = mode
    if mode == 2 then
        self.angle = 0
        self.pos = vec(pos.x, 0 - index * 30)
        self.update = self.update2
    end
end

function BugComponent:setEntity(entity)
    self.entity = entity
    self.entity:setPosition(self.pos)
    if self.mode == 1 then
        self.entity:setVelocity(vec(-1, 0))
    end
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
    return 8
end

function BugComponent:getY()
    local sina = math.sin(self.angle / math.pi)
    local y = math.floor(20 * sina)
    return self.initPosY + y
end

function BugComponent:update1()
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

function BugComponent:update2()
    local pos = self.entity:getPosition()
    local dy = 0
    local dx = 0
    local radius = 55
    if pos.y < 55 then
        dy = 1
        self.entity:setPosition(vec(pos.x + dx, pos.y + dy))
        self.pos = pos + vec(-radius, 0)
        self.entity:setFrame(4)
    elseif self.angle < 90 then
        self.angle = self.angle + 1.2
        if self.angle > 360 then
            self.angle = self.angle - 360
        end
        local cosa = math.cos(self.angle * math.pi / 180)
        local sina = math.sin(self.angle * math.pi / 180)
        self.entity:setPosition(self.pos + vec(-(0.5 * (1 + self.angle / 0.8)), 0) + vec(radius * cosa, radius * sina))

        local n = vec(cosa, sina)
        local angle = 180.0 * math.atan(n.y, n.x) / math.pi
        if angle < 0 then
            angle = angle + 360
        end
        self.entity:setFrame(self:getFrameFromAngle(angle))
    else
        dx = -1
        self.entity:setPosition(vec(pos.x + dx, pos.y + dy))
        self.entity:setFrame(8)
    end
end

return BugComponent
