local Component = require 'Component'
local DamageComponent = Component:subclass('DamageComponent')

DamageComponent.static.name = 'damage'

function DamageComponent:initialize(damage, invincible)
    Component.initialize(self)
    self.initialDamage = damage
    self.damage = damage
    self.invincible = invincible or false
end

function DamageComponent:getInitialDamage()
    return self.initialDamage
end

function DamageComponent:getDamage()
    return self.damage
end

function DamageComponent:isInvincible()
    return self.invincible
end

function DamageComponent:removeDamage(amount)
    if self:isInvincible() then return end
    self.damage = self.damage - amount
    if self.damage < 0 then
        self.damage = 0
    end
end

return DamageComponent
