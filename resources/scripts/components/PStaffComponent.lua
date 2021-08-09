local Component = require 'Component'
local PStaffComponent = Component:subclass('PStaffComponent')

PStaffComponent.static.name = 'pstaff'

function PStaffComponent:initialize()
    Component.initialize(self)
    self.count = 1
    self.leftDir = true
end

function PStaffComponent:getCount()
    return self.count
end

function PStaffComponent:increment()
    self.count = self.count + 1
end

function PStaffComponent:reset()
    self.count = 1
end

function PStaffComponent:setLeftDir(leftDir)
    self.leftDir = leftDir
end

function PStaffComponent:getLeftDir()
    return self.leftDir
end

return PStaffComponent
