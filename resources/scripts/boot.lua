-- set up package.path
package.path = package.path .. ';' .. getWorkingDirectory() .. '/resources/scripts/?.lua'

require 'EntityHandles'
require 'StateManager'
Keys = require 'Keys'

-- define enemy
local function createEnemy(name)
    print('Create', name)
    local e  = Entity()
    e:emplace('Name', {name=name})
    e:emplace('Position')
    e:emplace('Motion')
    e:emplace('Graphics')
    e:emplace('Collide', {size = vec(32, 12)})
    e:emplace('Animation', {name = 'resources/anims/enemy1.json'})
    setComponent(e, 'StateMachine', require 'EnemyStateMachine')
    setComponent(e, 'angle', 0)
    StateManager.initState(e)
    return e
end

-- define player
local function createPlayer()
    print('Create player')
    local e = Entity()
    e:emplace('Name', {name='player'})
    e:emplace('Position')
    e:emplace('Motion')
    e:emplace('Graphics')
    e:emplace('Collide', {size=vec(32, 12)})
    e:emplace('Animation', {name='resources/anims/spaceship.json'})
    setComponent(e, 'StateMachine', require('PlayerStateMachine'))
    setComponent(e, 'timer', 0)
    StateManager.initState(e)
    return e
end

-- create entities
createPlayer()
createEnemy('enemy1')
createEnemy('enemy2')

-- callbacks
function update()
    for key,value in pairs(Handles)
    do
        StateManager.update(value.cppRef)
    end
end

function onEvent(e, event)
    StateManager.onEvent(e, event)
end

function onKeyUp(code)
    for key,value in pairs(Handles)
    do
        StateManager.onKeyUp(value.cppRef, code)
    end
end

function onKeyDown(code)
    for key,value in pairs(Handles)
    do
        StateManager.onKeyDown(value.cppRef, code)
    end
end
