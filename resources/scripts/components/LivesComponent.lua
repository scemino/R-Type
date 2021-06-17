local Component = require 'Component'
local LivesComponent = Component:subclass('LivesComponent')

LivesComponent.static.name = 'lives'

function LivesComponent:initialize()
    Component.initialize(self)
    self.lives = 2
end

function LivesComponent:setLives(lives)
    if lives ~= self.lives then
        self.lives = lives
        local t = {}
        for _ = 1, lives do
            table.insert(t, 79)
        end
        getEntity('lives'):setTilesData(t)
    end
end

function LivesComponent:getLives()
    return self.lives
end

return LivesComponent
