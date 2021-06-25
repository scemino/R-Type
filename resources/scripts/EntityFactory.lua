local TimerComponent = require 'components.TimerComponent'
local BeamComponent = require 'components.BeamComponent'
local StateMachineComponent = require 'components.StateMachineComponent'
local EnemyPositionComponent = require 'components.EnemyPositionComponent'
local HealthComponent = require 'components.HealthComponent'
local DamageComponent = require 'components.DamageComponent'
local ForceComponent = require 'components.ForceComponent'
local LivesComponent = require 'components.LivesComponent'
local ScoreComponent = require 'components.ScoreComponent'
local TagComponent = require 'components.TagComponent'
local LaserComponent = require 'components.LaserComponent'
require 'components.BeamType'
require 'components.LaserDirection'

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

local function createForce()
    local e = Entity()
    e:emplace('Name', { name = 'force' })
    e:emplace('Position')
    e:emplace('Motion')
    e:emplace('Graphics')
    e:emplace('Hierarchy')
    e:emplace('Collide', { size = vec(32, 32) })
    e:emplace('Animation', { name = 'resources/anims/spaceship.json' })
    e:setAnim('force1', -1)
    e:setPosition(vec(32, 2))
    addComponent(e, ForceComponent())
    addComponent(e, DamageComponent(getDamageFromPower(1), true))
    addComponent(e, StateMachineComponent('states.ForceStateMachine'))
    StateManager.initState(e)
    return e
end

local function createForceBullet(name, pos, vel, anim)
    local e = Entity()
    e:emplace('Name', { name = name })
    e:emplace('Position')
    e:emplace('Motion')
    e:emplace('Graphics')
    e:emplace('Hierarchy')
    e:emplace('Collide', { size = vec(32, 12) })
    e:emplace('Animation', { name = 'resources/anims/spaceship.json' })
    e:setAnim(anim, -1)
    e:setVelocity(vel)
    e:setPosition(pos)
    addComponent(e, TagComponent('player_shoot'))
    addComponent(e, DamageComponent(getDamageFromPower(1)))
    addComponent(e, StateMachineComponent('states.ShootStateMachine'))
    StateManager.initState(e)
    getEntity('force'):addChild(e)
end

local function shootNormal(power, pos)
    print('shoot power ' .. power)
    local e = Entity()
    e:emplace('Name', { name = 'shoot' })
    e:emplace('Position')
    e:emplace('Motion')
    e:emplace('Graphics')
    e:emplace('Collide', { size = getHitBox(power) })
    e:emplace('Animation', { name = 'resources/anims/spaceship.json' })
    e:setAnim('shoot' .. power, -1)
    e:setVelocity(vec(12, 0))
    e:setPosition(pos)
    addComponent(e, DamageComponent(getDamageFromPower(power)))
    addComponent(e, StateMachineComponent('states.ShootStateMachine'))
    addComponent(e, TagComponent('player_shoot'))
    StateManager.initState(e)
end

local function shootRibbon(force, pos)
    print('shootRibbon')
    local forceLevel = force.components.force:getForceLevel()
    local isRear = force.components.force:isRear()
    local vel = force.components.force:getVelocity()

    if forceLevel == 2 then
        local offsetX = 32
        if isRear then
            offsetX = -48
        end
        local e = Entity()
        e:emplace('Name', { name = 'shoot1' })
        e:emplace('Position')
        e:emplace('Motion')
        e:emplace('Graphics')
        e:emplace('Collide', { size = getHitBox(2) })
        e:emplace('Animation', { name = 'resources/anims/spaceship.json' })
        e:setAnim('ribbon_small1', -1)
        e:setPosition(pos + vec(offsetX, -8))
        e:setVelocity(vel)
        e:setFlipX(isRear)
        addComponent(e, TagComponent('player_shoot'))
        addComponent(e, DamageComponent(getDamageFromPower(forceLevel)))
        addComponent(e, StateMachineComponent('states.ShootStateMachine'))
        StateManager.initState(e)

        e = Entity()
        e:emplace('Name', { name = 'shoot2' })
        e:emplace('Position')
        e:emplace('Motion')
        e:emplace('Graphics')
        e:emplace('Collide', { size = getHitBox(2) })
        e:emplace('Animation', { name = 'resources/anims/spaceship.json' })
        e:setAnim('ribbon_small2', -1)
        e:setPosition(pos + vec(offsetX, 8))
        e:setVelocity(vel)
        e:setFlipX(isRear)
        addComponent(e, TagComponent('player_shoot'))
        addComponent(e, DamageComponent(getDamageFromPower(forceLevel)))
        addComponent(e, StateMachineComponent('states.ShootStateMachine'))
        StateManager.initState(e)
    elseif forceLevel == 3 then
        local offsetX = 0
        if isRear then
            offsetX = -32
        end
        local e = Entity()
        e:emplace('Name', { name = 'shoot' })
        e:emplace('Position')
        e:emplace('Motion')
        e:emplace('Graphics')
        e:emplace('Collide', { size = getHitBox(3) })
        e:emplace('Animation', { name = 'resources/anims/spaceship.json' })
        e:setAnim('ribbon', -1)
        e:setPosition(pos + vec(offsetX, 0))
        e:setVelocity(vel)
        e:setFlipX(isRear)
        addComponent(e, TagComponent('player_shoot'))
        addComponent(e, DamageComponent(getDamageFromPower(forceLevel)))
        addComponent(e, StateMachineComponent('states.ShootStateMachine'))
        StateManager.initState(e)
    end
end

local function shootLaser(force, pos)
    local e = Entity()
    e:emplace('Name', { name = 'shoot1' })
    e:emplace('Position')
    e:emplace('Motion')
    e:emplace('Graphics')
    e:emplace('Collide', { size = getHitBox(2) })
    e:emplace('Animation', { name = 'resources/anims/spaceship.json' })
    e:setAnim('laser', 1)
    e:setFrame(0)
    e:setPosition(pos)
    e:setVelocity(vec(8, -9))
    addComponent(e, TagComponent('player_shoot'))
    addComponent(e, LaserComponent(LaserDirection.NE))
    addComponent(e, DamageComponent(getDamageFromPower(1)))
    addComponent(e, StateMachineComponent('states.LaserStateMachine'))
    StateManager.initState(e)

    e = Entity()
    e:emplace('Name', { name = 'shoot2' })
    e:emplace('Position')
    e:emplace('Motion')
    e:emplace('Graphics')
    e:emplace('Collide', { size = getHitBox(2) })
    e:emplace('Animation', { name = 'resources/anims/spaceship.json' })
    e:setAnim('laser', 1)
    e:setFrame(1)
    e:setPosition(pos)
    e:setVelocity(vec(8, 9))
    addComponent(e, TagComponent('player_shoot'))
    addComponent(e, LaserComponent(LaserDirection.SE))
    addComponent(e, DamageComponent(getDamageFromPower(1)))
    addComponent(e, StateMachineComponent('states.LaserStateMachine'))
    StateManager.initState(e)
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
        addComponent(e, LivesComponent())
        StateManager.initState(e)

        e:addChild(createBeam())
        createForce()
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
        local force = Handles[getEntity('force'):getId()]
        print('Shoot power ', power)
        if not force.components.force:isAttached() or power ~= 1 then
            beamType = BeamType.Normal
        end
        print('Shoot beam type ', beamType)

        shootNormal(power, pos)
        if beamType == BeamType.Ribbon then
            shootRibbon(force, pos)
        elseif beamType == BeamType.Laser then
            shootLaser(force, pos)
        end

        if power == 1 then
            playSound(Sounds.shoot1)
        else
            playSound(Sounds.shoot2)
        end

        local score = Handles[getEntity('score'):getId()].components.score
        score:setScore(score:getScore() + 1)
    end,

    forceShoot = function()
        local force = Handles[getEntity('force'):getId()]
        local pos = force:getPosition()
        local forceLevel = force.components.force:getForceLevel()

        -- East bullet
        if forceLevel == 1 then
            createForceBullet('force_bullet_e', pos, vec(6, 0), 'force_bullet_e')
        end

        if forceLevel < 2 then
            return
        end

        -- NE bullet
        createForceBullet('force_bullet_ne', pos, vec(6, -2), 'force_bullet_ne')

        -- SE bullet
        createForceBullet('force_bullet_se', pos, vec(6, 2), 'force_bullet_se')

        if forceLevel < 3 then
            return
        end

        -- North bullet
        createForceBullet('force_bullet_n', pos, vec(0, -4), 'force_bullet_n')

        -- South bullet
        createForceBullet('force_bullet_s', pos, vec(0, 4), 'force_bullet_s')
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
        addComponent(e, HealthComponent(1))
        addComponent(e, DamageComponent(1))
        addComponent(e, StateMachineComponent('states.EnemyBulletStateMachine'))
        StateManager.initState(e)
    end,

    createHUD = function()
        -- don't know yet if it's a good idea to use tiles
        local e = Entity()
        e:emplace('Name', { name = 'lives' })
        e:emplace('Position')
        e:emplace('Graphics')
        e:emplace('Tiles', { name = 'resources/tiles/hud.json' })
        e:setTilesData({ 79, 79 })
        e:setPosition(vec(8, 256))
        addComponent(e, StateMachineComponent('states.LivesIndicatorStateMachine'))
        StateManager.initState(e)

        e = Entity()
        e:emplace('Name', { name = '1P' })
        e:emplace('Position')
        e:emplace('Tiles', { name = 'resources/tiles/hud.json' })
        e:setTilesData({ 69, 70, 71, 72 })
        e:setPosition(vec(24, 264))

        e = Entity()
        e:emplace('Name', { name = 'beam_text' })
        e:emplace('Position')
        e:emplace('Tiles', { name = 'resources/tiles/hud.json' })
        e:setTilesData({ 57, 58, 59, 60, 61, 62 })
        e:setPosition(vec(80, 256))

        e = Entity()
        e:emplace('Name', { name = 'beam_level' })
        e:emplace('Position')
        e:emplace('Tiles', { name = 'resources/tiles/hud.json' })
        e:setPosition(vec(128, 256))

        e = Entity()
        e:emplace('Name', { name = 'score' })
        e:emplace('Position')
        e:emplace('Tiles', { name = 'resources/tiles/hud.json' })
        addComponent(e, ScoreComponent(0, vec(104, 264)))

        e = Entity()
        e:emplace('Name', { name = 'hi' })
        e:emplace('Position')
        e:emplace('Tiles', { name = 'resources/tiles/hud.json' })
        e:setPosition(vec(144, 264))
        e:setTilesData({ 76, 77, 78, 72 })

        -- ranking
        -- ABIKO 174500
        -- SUMITA 168600
        -- AKIO.O 159700
        -- SHINJI 117900
        -- MISAKO! 100500
        -- MASATO 98900
        -- HAMA 92000
        -- KENT.K 80000
        -- JIJEE 76000
        -- IREM . 75000
        e = Entity()
        e:emplace('Name', { name = 'hiscore' })
        e:emplace('Position')
        e:emplace('Tiles', { name = 'resources/tiles/hud.json' })
        addComponent(e, ScoreComponent(174500, vec(224, 264)))
    end
}

return EntityFactory