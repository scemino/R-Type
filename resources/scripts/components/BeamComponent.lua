require 'components.BeamType'
local Component = require 'Component'
local BeamComponent = Component:subclass('BeamComponent')

BeamComponent.static.name = 'beam'

local function getStart(power)
    if power == 0 then
        return 103
    elseif power == 1 then
        return 115
    elseif power == 2 then
        return 116
    elseif power == 3 then
        return 117
    else
        return 118
    end
end

local function getMiddle(power)
    local p = math.min(power, 8)
    if p == 0 then
        return 104
    else
        return 119 + p
    end
end

local function getEnd(power)
    if power <= 124 then
        return 105
    elseif power == 125 then
        return 128
    elseif power == 126 then
        return 129
    elseif power == 127 then
        return 130
    else
        return 131
    end
end

local function updateTiles(timer)
    local max = 128
    local power = math.floor(math.min(max * timer / 112, max))
    local t = { getStart(power) }
    for i = 0, 14 do
        table.insert(t, getMiddle(math.max(0, (power - 4) - i * 8)))
    end
    table.insert(t, getEnd(power))
    getEntity('beam_level'):setTilesData(t)
end

function BeamComponent:initialize()
    Component.initialize(self)
    self.timer = 0
    self.beamType = BeamType.Normal
    updateTiles(0)
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
    self.timer = self.timer + 2
    updateTiles(self.timer)
end


function BeamComponent:setEnabled(enabled)
    if not enabled then
        self.timer = 0
        updateTiles(self.timer)
    end
    self.enabled = enabled
end

function BeamComponent:getPower()
    local power = math.max(0, math.floor(self.timer / 4))
    power = math.min(5, math.floor(power / 4))
    return power + 1
end

return BeamComponent
