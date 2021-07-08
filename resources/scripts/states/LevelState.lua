local GameState = require 'states.GameState'
local LevelState = GameState:subclass('LevelState')

function LevelState:initialize()
    GameState.initialize(self)
end

function LevelState:update()
    spawnEnemies()
    for _, value in pairs(Handles)
    do
        if isHandleValid(value) then
            StateManager.update(value)
        end
    end
end

function LevelState:onEvent(e, event)
    if isHandleValid(e) then
        StateManager.onEvent(e, event)
    end
end

function LevelState:onKeyUp(key)
    for _, value in pairs(Handles)
    do
        if isHandleValid(value) then
            StateManager.onKeyUp(value, key)
        end
    end
end

function LevelState:onKeyDown(key)
    for _, value in pairs(Handles)
    do
        if isHandleValid(value) then
            StateManager.onKeyDown(value, key)
        end
    end
end

return LevelState
