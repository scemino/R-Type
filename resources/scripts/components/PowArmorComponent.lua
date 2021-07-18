local Component = require 'Component'
local PowArmorComponent = Component:subclass('PowArmorComponent')
PowArmorComponent.static.name = 'pow'

function PowArmorComponent:initialize(pos)
    Component.initialize(self)
    self.pos = pos
    self.timer = math.random()
end

function PowArmorComponent:setPosition(pos)
    self.pos = pos
end

function PowArmorComponent:update()
    if not self.alive then
        self.timer = self.timer + 0.02
    end
end

function PowArmorComponent:getY()
    return self.pos.y + 40 * math.sin(self.timer)
end

return PowArmorComponent
