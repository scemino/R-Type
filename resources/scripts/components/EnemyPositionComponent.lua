local Component = require 'Component'
local EnemyPositionComponent = Component:subclass('EnemyPositionComponent')

EnemyPositionComponent.static.name = 'enemyPosition'

function EnemyPositionComponent:initialize()
    Component.initialize(self)
    self.angle = 0
end

function EnemyPositionComponent:updatePosition(e)
    local x = self.entity:getPosition().x
    local y = math.floor(100 + 10 * math.sin(math.rad(self.angle)))
    self.entity:setPosition(vec(x, y))
    self.angle = self.angle + 5
    if self.angle >= 360 then
        self.angle = self.angle - 360
    end
end

return EnemyPositionComponent