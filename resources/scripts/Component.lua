local class = require 'lib.middleclass'

local Component = class("Component")

function Component:initialize()
end

function Component:setEntity(entity)
    self.entity = entity
end

return Component
