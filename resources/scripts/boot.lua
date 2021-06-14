-- set up package.path
package.path = package.path .. ';' .. getWorkingDirectory() .. '/resources/scripts/?.lua'

require 'EntityHandles'
require 'StateManager'
require 'Sounds'
require 'Keys'

local EnemyPositionComponent = require 'EnemyPositionComponent'
local TimerComponent = require 'TimerComponent'
local BeamComponent = require 'BeamComponent'
local StateMachineComponent = require 'StateMachineComponent'

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
    addComponent(e, StateMachineComponent('EnemyStateMachine'))
    addComponent(e, EnemyPositionComponent())
    StateManager.initState(e)
    return e
end

-- define player
local function createBeam()
    local beam = Entity()
    beam:emplace('Name', {name='beam'})
    beam:emplace('Position')
    beam:emplace('Graphics')
    beam:emplace('Hierarchy')
    beam:emplace('Animation', {name='resources/anims/spaceship.json'})
    beam:setAnim('beam', -1)
    beam:setVisible(false)
    beam:setPosition(vec(32,0))
    addComponent(beam, BeamComponent())
    addComponent(beam, StateMachineComponent('BeamStateMachine'))
    StateManager.initState(beam)
    return beam
end

local function createPlayer()
    print('Create player')
    local e = Entity()
    e:emplace('Name', {name='player'})
    e:emplace('Position')
    e:emplace('Motion')
    e:emplace('Graphics')
    e:emplace('Hierarchy')
    e:emplace('Collide', {size=vec(32, 12)})
    e:emplace('Animation', {name='resources/anims/spaceship.json'})
    addComponent(e, StateMachineComponent('PlayerStateMachine'))
    addComponent(e, TimerComponent())
    StateManager.initState(e)

    e:addChild(createBeam())
    return e
end

-- define camera
 local function createCamera()
     local e = Entity()
     e:emplace('Name', {name='camera'})
     e:emplace('Position')
     e:emplace('Motion')
     addComponent(e, StateMachineComponent('CameraStateMachine'))
     StateManager.initState(e)
     return e
 end

-- create entities
createCamera()
createPlayer()
createEnemy('enemy1')
createEnemy('enemy2')

-- callbacks
function update()
    for key,value in pairs(Handles)
    do
        if isHandleValid(value) then StateManager.update(value.cppRef) end
    end
end

function onEvent(e, event)
    StateManager.onEvent(e, event)
end

function onKeyUp(code)
    for key,value in pairs(Handles)
    do
        if isHandleValid(value) then StateManager.onKeyUp(value.cppRef, code) end
    end
end

function onKeyDown(code)
    for key,value in pairs(Handles)
    do
        if isHandleValid(value) then StateManager.onKeyDown(value.cppRef, code) end
    end
end
