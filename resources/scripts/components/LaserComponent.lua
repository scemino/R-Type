require 'components.LaserDirection'

local Component = require 'Component'
local LaserComponent = Component:subclass('LaserComponent')

LaserComponent.static.name = 'laser'

function LaserComponent:initialize(direction)
    Component.initialize(self)
    self.direction = direction
    self.timer = 0
    self.nextDirection = direction
end

function LaserComponent:update()
    if self.direction == self.nextDirection then
        return
    end
    self:setDirection(self.nextDirection)
end

function LaserComponent:flipX()
    print('flipX', self.direction)
    if self.direction == LaserDirection.NE then
        self.nextDirection = LaserDirection.NW
        self.entity:setVelocity(vec(-8, -9))
    elseif self.direction == LaserDirection.SE then
        self.nextDirection = LaserDirection.SW
        self.entity:setVelocity(vec(-8, 9))
    elseif self.direction == LaserDirection.NW then
        self.nextDirection = LaserDirection.NE
        self.entity:setVelocity(vec(8, -9))
    elseif self.direction == LaserDirection.SW then
        self.nextDirection = LaserDirection.SE
        self.entity:setVelocity(vec(8, 9))
    end
    --self.entity:setFrame(3)
    self:setDirection(self.nextDirection)
    --self.entity:setPosition(self.entity:getPosition() + vec(self.entity:getVelocity().x, 0))
end

function LaserComponent:flipY()
    print('flipY', self.direction)
    if self.direction == LaserDirection.NE then
        self.nextDirection = LaserDirection.SE
        self.entity:setVelocity(vec(8, 9))
    elseif self.direction == LaserDirection.SE then
        self.nextDirection = LaserDirection.NE
        self.entity:setVelocity(vec(8, -9))
    elseif self.direction == LaserDirection.NW then
        self.nextDirection = LaserDirection.SW
        self.entity:setVelocity(vec(-8, 9))
    elseif self.direction == LaserDirection.SW then
        self.nextDirection = LaserDirection.NW
        self.entity:setVelocity(vec(-8, -9))
    end
    --self.entity:setFrame(4)
    self:setDirection(self.nextDirection)
    --self.entity:setPosition(self.entity:getPosition() + vec(0, self.entity:getVelocity().y))
end

function LaserComponent:setDirection(direction)
    self.direction = direction
    if self.direction == LaserDirection.NE then
        print('set to NE')
        self.entity:setFrame(0)
    elseif self.direction == LaserDirection.SE then
        print('set to SE')
        self.entity:setFrame(1)
    elseif self.direction == LaserDirection.NW then
        print('set to NW')
        self.entity:setFrame(1)
    elseif self.direction == LaserDirection.SW then
        print('set to SW')
        self.entity:setFrame(0)
    end
end

function LaserComponent:getDirection()
    return self.direction
end

return LaserComponent
