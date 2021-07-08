local enemies = {}
local objs = getLevelObjs()
local gameWidth = 384
local group = 0
local camera

function initEnemies()
    camera = getEntity('camera')
    for i, o in pairs(objs) do
        print(o)
        enemies[i] = true
    end
end

function spawnEnemies()
    local cx = camera:getPosition().x + gameWidth
    for i, o in pairs(objs) do
        if enemies[i] then
            local x = math.floor(o.pos.x)
            -- spawn enemies by wave (specified by a group id)
            if x == cx or group == o.group then
                group = o.group
                EntityFactory.createEnemy(o.name, o.pos)
                enemies[i] = false
            end
        end
    end
end