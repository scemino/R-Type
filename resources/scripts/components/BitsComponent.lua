BitsType = {
    Up = 1,
    Down = 2
}

local Component = require 'Component'
local BitsComponent = Component:subclass('BitsComponent')

BitsComponent.static.name = 'bits'

function BitsComponent:initialize(bitsType)
    Component.initialize(self)
    self.bitsType = bitsType
end

function BitsComponent:getBitsType()
    return self.bitsType
end

return BitsComponent
