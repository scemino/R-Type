local Component = require 'Component'
local HealthComponent = Component:subclass('HealthComponent')

HealthComponent.static.name = 'health'

function HealthComponent:initialize(initialHealth)
    Component.initialize(self)
    self.health = initialHealth
end

function HealthComponent:isAlive()
    return self.health > 0
end

function HealthComponent:getHealth()
    return self.health
end

function HealthComponent:setDamage(damage)
    self.health = self.health - damage
    if self.health < 0 then
        self.health = 0
    end
end

return HealthComponent
