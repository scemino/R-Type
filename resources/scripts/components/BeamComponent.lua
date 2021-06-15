require 'components.BeamType'
local Component = require 'Component'
local BeamComponent = Component:subclass('BeamComponent')

BeamComponent.static.name = 'beam'

function BeamComponent:initialize()
    Component.initialize(self)
    self.timer = 0
    self.beamType = BeamType.Normal
end

function BeamComponent:setBeamType(beamType)
    self.beamType = beamType
end

function BeamComponent:getBeamType()
    return self.beamType
end

function BeamComponent:update()
    if not self.enabled then
        return
    end
    self.timer = self.timer + 1
end

function BeamComponent:setEnabled(enabled)
    if not enabled then
        self.timer = 0
    end
    self.enabled = enabled
end

function BeamComponent:getPower()
    local power = math.max(0, math.floor(self.timer / 4))
    power = math.min(5, math.floor(power / 4))
    return power + 1
end

return BeamComponent
