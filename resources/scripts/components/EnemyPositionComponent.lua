local Component = require 'Component'
local EnemyPositionComponent = Component:subclass('EnemyPositionComponent')

EnemyPositionComponent.static.name = 'enemyPosition'

function EnemyPositionComponent:initialize(pos)
    Component.initialize(self)
    self.angle = 0
    self.initPosY = pos.y
    self.r = math.random(1,10)
end

function EnemyPositionComponent:updatePosition(e)
    local x = self.entity:getPosition().x
    local y = math.floor(10 * math.sin(math.rad(self.angle) + self.r))
    self.entity:setPosition(vec(x, self.initPosY + y))
    self.angle = self.angle + 5
    if self.angle >= 360 then
        self.angle = self.angle - 360
    end
end

return EnemyPositionComponent
