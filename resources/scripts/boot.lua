-- set up package.path
package.path = package.path .. ';' .. getWorkingDirectory() .. '/resources/scripts/?.lua'

require 'EntityHandles'
require 'StateManager'
require 'Sounds'
require 'Keys'
require 'EntityFactory'
require 'EnemySpawning'

local hitboxesVisible = false
showHitboxes(hitboxesVisible)

-- create entities
EntityFactory.createCamera()
EntityFactory.createHUD()
EntityFactory.createPlayer()
initEnemies()

-- callbacks
function update()
    spawnEnemies()
    for _, value in pairs(Handles)
    do
        if isHandleValid(value) then
            StateManager.update(value)
        end
    end
end

function onEvent(e, event)
    local handle = Handles[e:getId()]
    if isHandleValid(handle) then
        StateManager.onEvent(handle, event)
    end
end

function onKeyUp(code)
    for _, value in pairs(Handles)
    do
        if isHandleValid(value) then
            StateManager.onKeyUp(value, code)
        end
    end
end

function onKeyDown(code)
    if code == Keys.H then
        hitboxesVisible = not hitboxesVisible
        showHitboxes(hitboxesVisible)
    end

    for _, value in pairs(Handles)
    do
        if isHandleValid(value) then
            StateManager.onKeyDown(value, code)
        end
    end
end
