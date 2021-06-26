local Component = require 'Component'
local ForceComponent = Component:subclass('ForceComponent')

ForceComponent.static.name = 'force'

ForceAttachment = {
    None = 0,
    Rear = 1,
    Front = 2
}

function ForceComponent:initialize()
    Component.initialize(self)
    self.forceLevel = 1
    self.attached = ForceAttachment.None
end

function ForceComponent:setForceLevel(forceLevel)
    self.forceLevel = forceLevel
    if self:isRear() and forceLevel == 2 then
        self.entity:setAnim('force2_rear', -1)
    else
        self.entity:setAnim('force' .. forceLevel, -1)
    end
end

function ForceComponent:getForceLevel()
    return self.forceLevel
end

function ForceComponent:setAttachment(forceAttachment)
    self.forceAttachment = forceAttachment
    if forceAttachment ~= ForceAttachment.None then
        self:setForceLevel(self.forceLevel)
    end
end

function ForceComponent:isAttached()
    return self.forceAttachment ~= ForceAttachment.None
end

function ForceComponent:isRear()
    return self.forceAttachment == ForceAttachment.Rear
end

function ForceComponent:isFront()
    return self.forceAttachment == ForceAttachment.Front
end

function ForceComponent:getVelocity()
    if self:isRear() then
        return vec(-6, 0)
    end
    return vec(6, 0)
end

function ForceComponent:getAttachment()
    return self.forceAttachment
end

return ForceComponent
