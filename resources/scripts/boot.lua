-- set up package.path
package.path = package.path .. ';' .. getWorkingDirectory() .. '/resources/scripts/?.lua'

require 'EntityHandles'
require 'StateManager'
require 'Sounds'
require 'Keys'
require 'EntityFactory'

-- create entities
EntityFactory.createCamera()
EntityFactory.createPlayer()
--for i = 1, 7 do
--    EntityFactory.createEnemy('enemy'..i, vec(60+40*i, 100))
--end

-- callbacks
function update()
    for _,value in pairs(Handles)
    do
        if isHandleValid(value) then StateManager.update(value) end
    end
end

function onEvent(e, event)
    local handle = Handles[e:getId()]
    if isHandleValid(handle) then StateManager.onEvent(handle, event) end
end

function onKeyUp(code)
    for _,value in pairs(Handles)
    do
        if isHandleValid(value) then StateManager.onKeyUp(value, code) end
    end
end

function onKeyDown(code)
    for _,value in pairs(Handles)
    do
        if isHandleValid(value) then StateManager.onKeyDown(value, code) end
    end
end
