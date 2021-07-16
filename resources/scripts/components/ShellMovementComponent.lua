local Component = require 'Component'
local ShellMovementComponent = Component:subclass('ShellMovementComponent')

ShellMovementComponent.static.N = 18
ShellMovementComponent.static.STEP = 360 / ShellMovementComponent.static.N
ShellMovementComponent.static.name = 'shellItem'

local function getFrameIndex(angle)
    return math.floor((12 + (angle // 22.5)) % 16)
end

function ShellMovementComponent:initialize(radius, index)
    Component.initialize(self)
    self.radius = radius
    self.angle = ShellMovementComponent.static.STEP * index
    self.index = index
    self.alive = true
    self.frameOffset = 0
    if self:isMain() then
        self.frameOffset = 16
    end
end

function ShellMovementComponent:die()
    print('die shell item', self.index)
    self.alive = false
    self.entity:setFrame(36)
end

function ShellMovementComponent:getAngle()
    return self.angle
end

function ShellMovementComponent:getIndex()
    return self.index
end

function ShellMovementComponent:isMain()
    return self.index == ShellMovementComponent.static.N - 5
end

function ShellMovementComponent:update()
    self.angle = self.angle - 0.4
    if self.angle < 0 then
        self.angle = self.angle + 360
    end
    local angleRad = self.angle * math.pi / 180
    local x = self.radius * math.cos(angleRad)
    local y = self.radius * math.sin(angleRad)
    local shellPos = getEntity('shell'):getPosition()
    self.entity:setPosition(shellPos + vec(x, y))
    if self.alive then
        self.entity:setFrame(self.frameOffset + getFrameIndex(self.angle))
    end
end

return ShellMovementComponent
