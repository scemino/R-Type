local Component = require 'Component'
local ShellComponent = Component:subclass('ShellComponent')
ShellComponent.static.name = 'shell'

function ShellComponent:initialize()
    Component.initialize(self)
    self.alive = true
    self.timer = 0.0
end

function ShellComponent:isAlive()
    return self.alive
end

function ShellComponent:die()
    print('die shell')
    self.alive = false
end

function ShellComponent:isDead(itemIndex)
    local index = math.floor(self.timer)
    if itemIndex == 13 - index or itemIndex == 13 + index then
        return true
    end
    return false
end

function ShellComponent:update()
    if not self.alive then
        self.timer = self.timer + 0.1
    end
end

return ShellComponent
