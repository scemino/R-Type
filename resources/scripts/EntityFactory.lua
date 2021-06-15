local TimerComponent = require 'components.TimerComponent'
local BeamComponent = require 'components.BeamComponent'
local StateMachineComponent = require 'components.StateMachineComponent'
local EnemyPositionComponent = require 'components.EnemyPositionComponent'
local HealthComponent = require 'components.HealthComponent'
local DamageComponent = require 'components.DamageComponent'
require 'components.BeamType'

local hitBoxes = {
    vec(16, 4),
    vec(16, 8),
    vec(32, 10),
    vec(48, 12),
    vec(64, 14),
    vec(80, 14),
}

local function getHitBox(power)
    return hitBoxes[power]
end

local function getDamageFromPower(power)
    return 20 + (power - 1) * 100
end

local function createBeam()
    local beam = Entity()
    beam:emplace('Name', { name = 'beam' })
    beam:emplace('Position')
    beam:emplace('Graphics')
    beam:emplace('Hierarchy')
    beam:emplace('Animation', { name = 'resources/anims/spaceship.json' })
    beam:setAnim('beam', -1)
    beam:setVisible(false)
    beam:setPosition(vec(16, 2))
    addComponent(beam, BeamComponent())
    addComponent(beam, StateMachineComponent('states.BeamStateMachine'))
    StateManager.initState(beam)
    return beam
end

EntityFactory = {
    createPlayer = function()
        print('Create player')
        local e = Entity()
        e:emplace('Name', { name = 'player' })
        e:emplace('Position')
        e:emplace('Motion')
        e:emplace('Graphics')
        e:emplace('Hierarchy')
        e:emplace('Collide', { size = vec(32, 12) })
        e:emplace('Animation', { name = 'resources/anims/spaceship.json' })
        addComponent(e, StateMachineComponent('states.PlayerStateMachine'))
        addComponent(e, TimerComponent())
        StateManager.initState(e)

        e:addChild(createBeam())
        return e
    end,

    createEnemy = function(name, pos)
        print('Create', name)
        local e = Entity()
        e:emplace('Name', { name = name })
        e:emplace('Position')
        e:emplace('Motion')
        e:emplace('Graphics')
        e:emplace('Collide', { size = vec(32, 32) })
        e:emplace('Animation', { name = 'resources/anims/enemy1.json' })
        e:setPosition(pos)
        addComponent(e, StateMachineComponent('states.EnemyStateMachine'))
        addComponent(e, EnemyPositionComponent())
        addComponent(e, HealthComponent(20))
        StateManager.initState(e)
        return e
    end,

    createCamera = function()
        local e = Entity()
        e:emplace('Name', { name = 'camera' })
        e:emplace('Position')
        e:emplace('Motion')
        addComponent(e, StateMachineComponent('states.CameraStateMachine'))
        StateManager.initState(e)
        return e
    end,

    shoot = function(power, beamType, pos)
        print('shoot power ' .. power)
        local e = Entity()
        e:emplace('Name', { name = 'shoot' })
        e:emplace('Position')
        e:emplace('Motion')
        e:emplace('Graphics')
        e:emplace('Collide', { size = getHitBox(power) })
        e:emplace('Animation', { name = 'resources/anims/spaceship.json' })
        if beamType == BeamType.Ribbon and power == 1 then
            e:setAnim('ribbon', -1)
            e:setVelocity(vec(6, 0))
        else
            e:setAnim('shoot' .. power, -1)
            e:setVelocity(vec(12, 0))
        end
        e:setPosition(pos)
        addComponent(e, DamageComponent(getDamageFromPower(power)))
        addComponent(e, StateMachineComponent('states.ShootStateMachine'))
        StateManager.initState(e)
        if power == 1 then
            playSound(Sounds.shoot1)
        else
            playSound(Sounds.shoot2)
        end
    end,

    enemyShoot = function(pos)
        -- enemy shoot in direction of the player
        local player = getEntity('player')
        local playerPos = player:getPosition()
        local dX = playerPos.x - pos.x;
        local dY = playerPos.y - pos.y;
        local d = math.sqrt(dX * dX + dY * dY)

        local e = Entity()
        e:emplace('Name', { name = 'enemy_bullet' })
        e:emplace('Position')
        e:emplace('Motion')
        e:emplace('Graphics')
        e:emplace('Collide', { size = vec(4, 4) })
        e:emplace('Animation', { name = 'resources/anims/enemy_bullet.json' })
        e:setPosition(pos)
        e:setVelocity(vec(2 * dX / d, 2 * dY / d))
        addComponent(e, StateMachineComponent('states.EnemyBulletStateMachine'))
        StateManager.initState(e)
    end
}

return EntityFactory