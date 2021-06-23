local Component = require 'Component'
local TagComponent = Component:subclass('TagComponent')

TagComponent.static.name = 'tag'

function TagComponent:initialize(tag)
    Component.initialize(self)
    self.tag = tag
end

function TagComponent:setTag(tag)
    self.tag = tag
end

function TagComponent:getTag()
    return self.tag
end

return TagComponent
