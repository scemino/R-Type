local Component = require 'Component'
local EnemyPositionComponent = Component:subclass('EnemyPositionComponent')

EnemyPositionComponent.static.name = 'enemyPosition'

function EnemyPositionComponent:initialize()
    Component.initialize(self)
    self.angle = 0
end

function EnemyPositionComponent:updatePosition()
    local x = self.entity:getPosition().x
    local y = math.floor(100+10*math.sin(math.rad(self.angle)))
    self.entity:setPosition(vec(x,y))
    self.angle = self.angle+5
end

return EnemyPositionComponent
