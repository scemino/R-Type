local Component = require 'Component'
local ForceComponent = Component:subclass('ForceComponent')

ForceComponent.static.name = 'force'

function ForceComponent:initialize()
    Component.initialize(self)
    self.forceLevel = 1
    self.attached = false
end

function ForceComponent:setForceLevel(forceLevel)
    self.forceLevel = forceLevel
    self.entity:setAnim('force'..forceLevel, -1)
end

function ForceComponent:getForceLevel()
    return self.forceLevel
end

function ForceComponent:setAttached(attached)
    self.attached = attached
end

function ForceComponent:isAttached()
    return self.attached
end

return ForceComponent
