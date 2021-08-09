local Component = require 'Component'
local MissileComponent = Component:subclass('MissileComponent')

MissileComponent.static.name = 'missile'
MissileComponent.static.G = 9.81

function MissileComponent:initialize(leftDir, pos)
    Component.initialize(self)
    self.time = 0
    self.initPos = pos
    self.vel = math.random(45, 55)
    if leftDir then
        self.angle = math.random(95, 105) * math.pi / 180.0
    else
        self.angle = math.random(70, 85) * math.pi / 180.0
    end
    self.leftDir = leftDir
end

function MissileComponent:getIndexFromAngle(theta)
    if theta > 67 then
        return 0
    elseif theta > 45 then
        return 1
    elseif theta > 23 then
        return 2
    elseif theta > 0 then
        return 3
    elseif theta > -23 then
        return 4
    elseif theta > -45 then
        return 3
    elseif theta > -67 then
        return 2
    elseif theta > -90 then
        return 1
    else
        return 0
    end
end

function MissileComponent:getFlipY(theta)
    if self.leftDir then
        return theta < 0
    else
        return theta > 0
    end
end

function MissileComponent:update()
    self.time = self.time + 1
    local t = self.time / 10.0

    -- the velocity vector
    local Vsin = self.vel * math.sin(self.angle)
    local Vx = self.vel * math.cos(self.angle)
    local Vy = MissileComponent.static.G * t - Vsin

    -- the position
    local x = Vx * t
    local y = MissileComponent.static.G * t * t / 2.0 - Vsin * t

    -- the angle between the ground and the velocity vector
    local theta = math.atan(Vy / Vx) * 180.0 / math.pi

    -- update sprite
    local frameIndex = self:getIndexFromAngle(theta)
    self.entity:setFrame(frameIndex)
    self.entity:setFlipY(self:getFlipY(theta))
    self.entity:setPosition(self.initPos + vec(x, y))
end

return MissileComponent
