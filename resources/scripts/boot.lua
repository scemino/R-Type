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
--EntityFactory.createEnemy('pow', vec(500, 180))
EntityFactory.createPowArmor('pow', vec(600, 180), ItemType.Ribbon)
EntityFactory.createPowArmor('pow', vec(700, 180), ItemType.Ribbon)
EntityFactory.createPowArmor('pow', vec(800, 180), ItemType.Ribbon)
EntityFactory.createPowArmor('pow', vec(1600, 180), ItemType.Bits)
EntityFactory.createPowArmor('pow', vec(1800, 180), ItemType.Ribbon)
EntityFactory.createPowArmor('pow', vec(2200, 180), ItemType.Ribbon)
EntityFactory.createPowArmor('pow', vec(2400, 220), ItemType.Bits)
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
