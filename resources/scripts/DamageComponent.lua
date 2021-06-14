local Component = require 'Component'
local DamageComponent = Component:subclass('DamageComponent')

DamageComponent.static.name = 'damage'

function DamageComponent:initialize(damage)
    Component.initialize(self)
    self.damage = damage
end

function DamageComponent:getDamage()
    return self.damage
end

return DamageComponent
