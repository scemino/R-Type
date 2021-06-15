local Component = require 'Component'
local DamageComponent = Component:subclass('DamageComponent')

DamageComponent.static.name = 'damage'

function DamageComponent:initialize(damage)
    Component.initialize(self)
    self.initialDamage = damage
    self.damage = damage
end

function DamageComponent:getInitialDamage()
    return self.initialDamage
end

function DamageComponent:getDamage()
    return self.damage
end

function DamageComponent:removeDamage(amount)
    self.damage = self.damage - amount
    if self.damage < 0 then
        self.damage = 0
    end
end

return DamageComponent
