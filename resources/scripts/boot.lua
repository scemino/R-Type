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
EntityFactory.createEnemy('p-staff', vec(700, 223))
EntityFactory.createEnemy('tabrok', vec(200, 100))
--EntityFactory.createEnemy('cancer', vec(1900, 20))
--EntityFactory.createEnemy('cancer', vec(1910, 20))
--EntityFactory.createEnemy('cancer', vec(1920, 20))
--EntityFactory.createEnemy('cancer', vec(1930, 20))
--EntityFactory.createEnemy('cancer', vec(1950, 20))
--EntityFactory.createEnemy('cancer', vec(1900, 20))
--EntityFactory.createEnemy('cancer', vec(1950, 20))
--EntityFactory.createEnemy('cancer', vec(1900, 20))
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
