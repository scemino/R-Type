local enemies = {}
local objs = getLevelObjs()
local gameWidth = 384
local group
local camera

function initEnemies()
    camera = getEntity('camera')
    for i, o in pairs(objs) do
        print(o)
        enemies[i] = true
    end
end

function spawnEnemies()
    local cx1 = camera:getPosition().x
    local cx2 = cx1 + gameWidth
    for i, o in pairs(objs) do
        if enemies[i] then
            local x = math.floor(o.pos.x)
            -- spawn enemies by wave (specified by a group id)
            local grp = o.properties.Group
            if x == cx2 or (group and group == grp) then
                if grp then
                    group = grp
                end
                EntityFactory.createEnemy(o.name, vec(o.pos.x - cx1, o.pos.y), o)
                enemies[i] = false
            end
        end
    end
end