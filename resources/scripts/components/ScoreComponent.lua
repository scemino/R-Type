local Component = require 'Component'
local ScoreComponent = Component:subclass('ScoreComponent')

ScoreComponent.static.name = 'score'

local function tablelength(T)
    local count = 0
    for _ in pairs(T) do count = count + 1 end
    return count
end

local function updateTiles(e, score, pos)
    local t = { }
    while true
    do
        local d = math.fmod(score, 10)
        table.insert(t, 1, d)
        if score < 10 then
            break
        end
        score = math.floor(score / 10)
    end
    local size = tablelength(t) * 8
    e:setPosition(vec(pos.x + 8 - size, pos.y))
    e:setTilesData(t)
end

function ScoreComponent:initialize(initialScore, position)
    Component.initialize(self)
    self.score = initialScore
    self.position = position
end

function ScoreComponent:setEntity(entity)
    Component.setEntity(self, entity)
    updateTiles(self.entity, self.score, self.position)
end

function ScoreComponent:setScore(score)
    self.score = score
    updateTiles(self.entity, score, self.position)
end

function ScoreComponent:getScore()
    return self.score
end

return ScoreComponent
