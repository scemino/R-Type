local Component = require 'Component'
local ItemComponent = Component:subclass('ItemComponent')

ItemComponent.static.name = 'item'

function ItemComponent:initialize(itemType)
    Component.initialize(self)
    self.itemType = itemType
end

function ItemComponent:getItemType()
    return self.itemType
end

return ItemComponent
