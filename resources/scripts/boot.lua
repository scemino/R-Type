-- set up package.path
package.path = package.path .. ';' .. getWorkingDirectory() .. '/resources/scripts/?.lua'

require 'EntityHandles'
require 'StateManager'
require 'Sounds'
require 'Keys'
require 'EntityFactory'
require 'EnemySpawning'
require 'actions.Actions'
require 'components.ItemType'
local Cutscenes = require 'Cutscenes.Cutscenes'
local LevelState = require 'states.LevelState'
local ThreadManager = require 'ThreadManager'

local hitboxesVisible = false
showHitboxes(hitboxesVisible)
tm = ThreadManager()

-- create entities
EntityFactory.createCamera()
EntityFactory.createHUD()
EntityFactory.createPlayer()
initEnemies()
Cutscenes.intro()

local levelState = LevelState()

-- callbacks
function update()
    levelState:update()
    tm:update()
end

function onEvent(e, event)
    levelState:onEvent(e, event)
end

function onKeyUp(code)
    levelState:onKeyUp(code)
end

function onKeyDown(code)
    if code == Keys.H then
        hitboxesVisible = not hitboxesVisible
        showHitboxes(hitboxesVisible)
    end
    levelState:onKeyDown(code)
end
