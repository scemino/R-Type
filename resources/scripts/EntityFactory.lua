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
local BitsComponent = require 'components.BitsComponent'
local ShellComponent = require 'components.ShellComponent'
local ShellMovementComponent = require 'components.ShellMovementComponent'
local PowArmorComponent = require 'components.PowArmorComponent'
local ItemComponent = require 'components.ItemComponent'
local PStaffComponent = require 'components.PStaffComponent'
require 'components.BeamType'
require 'components.LaserDirection'
local util = require 'util'

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
    --print('shoot power ' .. power)
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

local function shootLaser(pos)
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
        e:addChild(EntityFactory.createBoost(e))
        return e
    end,

    createForce = function()
        print('Create force')
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
        e:setZOrder(10)
        addComponent(e, ForceComponent())
        addComponent(e, DamageComponent(getDamageFromPower(1), true))
        addComponent(e, StateMachineComponent('states.ForceStateMachine'))
        StateManager.initState(e)
        return e
    end,

    createBitsUp = function(pos)
        print('Create bits')
        local e = Entity()
        e:emplace('Name', { name = 'bits_up' })
        e:emplace('Position')
        e:emplace('Motion')
        e:emplace('Graphics')
        e:emplace('Hierarchy')
        e:emplace('Collide', { size = vec(18, 18) })
        e:emplace('Animation', { name = 'resources/anims/spaceship.json' })
        e:setAnim('bits', -1)
        e:setPosition(pos)
        addComponent(e, BitsComponent(BitsType.Up))
        addComponent(e, DamageComponent(getDamageFromPower(1), true))
        addComponent(e, StateMachineComponent('states.BitsStateMachine'))
        StateManager.initState(e)
    end,

    createBitsDown = function(pos)
        print('Create bits')
        local e = Entity()
        e:emplace('Name', { name = 'bits_dn' })
        e:emplace('Position')
        e:emplace('Motion')
        e:emplace('Graphics')
        e:emplace('Hierarchy')
        e:emplace('Collide', { size = vec(18, 18) })
        e:emplace('Animation', { name = 'resources/anims/spaceship.json' })
        e:setAnim('bits', -1)
        e:setPosition(pos)
        addComponent(e, BitsComponent(BitsType.Down))
        addComponent(e, DamageComponent(getDamageFromPower(1), true))
        addComponent(e, StateMachineComponent('states.BitsStateMachine'))
        StateManager.initState(e)
    end,

    createBoost = function()
        local e = Entity()
        e:emplace('Name', { name = 'boost' })
        e:emplace('Position')
        e:emplace('Motion')
        e:emplace('Graphics')
        e:emplace('Hierarchy')
        e:emplace('Collide', { size = vec(32, 32) })
        e:emplace('Animation', { name = 'resources/anims/spaceship.json' })
        e:setAnim('boost', -1)
        e:setPosition(vec(-32, 2))
        return e
    end,

    createBlaster = function(name, pos)
        local e = Entity()
        e:emplace('Name', { name = name })
        e:emplace('Position')
        e:emplace('Motion')
        e:emplace('Graphics')
        e:emplace('Hierarchy')
        e:emplace('Collide', { size = vec(32, 32) })
        e:emplace('Animation', { name = 'resources/anims/blaster.json' })
        e:setFrame(0)
        e:setPosition(pos)
        addComponent(e, StateMachineComponent('states.BlasterStateMachine'))
        addComponent(e, HealthComponent(20))
        StateManager.initState(e)
        getEntity('level'):addChild(e)
        print('Create', e:getName(), pos)
        return e
    end,

    createBink = function(name, pos)
        local e = Entity()
        e:emplace('Name', { name = name })
        e:emplace('Position')
        e:emplace('Motion')
        e:emplace('Graphics')
        e:emplace('Hierarchy')
        e:emplace('Collide', { size = vec(32, 32) })
        e:emplace('Animation', { name = 'resources/anims/bink.json' })
        e:setPosition(pos)
        addComponent(e, StateMachineComponent('states.BinkStateMachine'))
        addComponent(e, HealthComponent(20))
        StateManager.initState(e)
        getEntity('level'):addChild(e)
        print('Create', e:getName(), pos)
        return e
    end,

    createPStaff = function(name, pos)
        local e = Entity()
        e:emplace('Name', { name = name })
        e:emplace('Position')
        e:emplace('Motion')
        e:emplace('Graphics')
        e:emplace('Hierarchy')
        e:emplace('Collide', { box = { 0, -32, 32, 32 } })
        --e:emplace('Collide', { size = vec(48, 44) })
        e:emplace('Animation', { name = 'resources/anims/p_staff.json' })
        e:setPosition(pos)
        addComponent(e, StateMachineComponent('states.PStaffStateMachine'))
        addComponent(e, HealthComponent(120))
        addComponent(e, PStaffComponent())
        StateManager.initState(e)
        getEntity('level'):addChild(e)
        print('Create', e:getName(), pos)
        return e
    end,

    createPowArmor = function(name, pos, itemType)
        local e = Entity()
        e:emplace('Name', { name = name })
        e:emplace('Position')
        e:emplace('Motion')
        e:emplace('Graphics')
        e:emplace('Hierarchy')
        e:emplace('Collide', { size = vec(32, 32) })
        e:emplace('Animation', { name = 'resources/anims/pow_armor.json' })
        e:setPosition(pos)
        addComponent(e, StateMachineComponent('states.PowArmorStateMachine'))
        addComponent(e, HealthComponent(20))
        addComponent(e, PowArmorComponent(pos))
        addComponent(e, ItemComponent(itemType))
        StateManager.initState(e)
        getEntity('level'):addChild(e)
        print('Create', e:getName(), pos, itemType)
        return e
    end,

    createItem = function(pos, itemType)
        local e = Entity()
        e:emplace('Name', { name = 'item' })
        e:emplace('Position')
        e:emplace('Graphics')
        e:emplace('Hierarchy')
        e:emplace('Collide', { size = vec(32, 32) })
        e:emplace('Animation', { name = 'resources/anims/pow_armor.json' })
        e:setAnim('items', 1)
        e:setFrame(0)
        e:setPosition(pos)
        addComponent(e, StateMachineComponent('states.ItemStateMachine'))
        addComponent(e, ItemComponent(itemType))
        StateManager.initState(e)
        getEntity('level'):addChild(e)
        print('Create', e:getName(), pos, itemType)
        return e
    end,

    createScant = function(name, pos)
        local e = Entity()
        e:emplace('Name', { name = name })
        e:emplace('Position')
        e:emplace('Motion')
        e:emplace('Graphics')
        e:emplace('Hierarchy')
        e:emplace('Collide', { size = vec(48, 44) })
        e:emplace('Animation', { name = 'resources/anims/scant.json' })
        e:setPosition(pos)
        addComponent(e, HealthComponent(220))
        addComponent(e, StateMachineComponent('states.ScantStateMachine'))
        StateManager.initState(e)
        print('Create', e:getName(), pos)
        return e
    end,

    createCancer = function(name, pos)
        local e = Entity()
        e:emplace('Name', { name = name })
        e:emplace('Position')
        e:emplace('Motion')
        e:emplace('Graphics')
        e:emplace('Hierarchy')
        e:emplace('Collide', { size = vec(48, 44) })
        e:emplace('Animation', { name = 'resources/anims/cancer.json' })
        e:setPosition(pos)
        getEntity('level'):addChild(e)
        addComponent(e, HealthComponent(20))
        addComponent(e, StateMachineComponent('states.CancerStateMachine'))
        StateManager.initState(e)
        print('Create', e:getName(), pos)
        return e
    end,

    createPataPataEnemy = function(name, pos)
        local e = Entity()
        e:emplace('Name', { name = name })
        e:emplace('Position')
        e:emplace('Motion')
        e:emplace('Graphics')
        e:emplace('Collide', { size = vec(32, 32) })
        e:emplace('Animation', { name = 'resources/anims/enemy1.json' })
        e:setPosition(pos)
        e:setVelocity(vec(-2, 0))
        addComponent(e, TagComponent('enemy'))
        addComponent(e, StateMachineComponent('states.EnemyStateMachine'))
        addComponent(e, EnemyPositionComponent(pos))
        addComponent(e, HealthComponent(20))
        StateManager.initState(e)
        print('Create', e:getName(), pos)
        return e
    end,

    createEnemy = function(name, pos)
        if name == 'enemy1' then
            return EntityFactory.createPataPataEnemy(name, pos)
        elseif name == 'blaster' then
            return EntityFactory.createBlaster(name, pos)
        elseif name == 'shell' then
            return EntityFactory.createEnemyShell(80, pos)
        elseif name == 'bink' then
            return EntityFactory.createBink(name, pos)
        elseif name == 'scant' then
            return EntityFactory.createScant(name, pos)
        elseif name == 'pow' then
            return EntityFactory.createPowArmor(name, pos)
        elseif name == 'cancer' then
            return EntityFactory.createCancer(name, pos)
        elseif name == 'p-staff' then
            return EntityFactory.createPStaff(name, pos)
        end
    end,

    createEnemyShell = function(radius, pos)
        local shell = Entity()
        shell:emplace('Name', { name = 'shell' })
        shell:emplace('Position')
        shell:emplace('Hierarchy')
        shell:setPosition(pos)
        addComponent(shell, ShellComponent())
        addComponent(shell, StateMachineComponent('states.ShellStateMachine'))
        addComponent(shell, TagComponent('enemy'))
        StateManager.initState(shell)
        for i = 1, ShellMovementComponent.static.N - 2 do
            local e = Entity()
            e:emplace('Name', { name = 'shell' .. i })
            e:emplace('Position')
            e:emplace('Motion')
            e:emplace('Graphics')
            e:emplace('Hierarchy')
            e:emplace('Collide', { size = vec(32, 32) })
            e:emplace('Animation', { name = 'resources/anims/shell.json' })
            e:setPosition(pos)
            shell:addChild(e)
            addComponent(e, TagComponent('enemy'))
            addComponent(e, HealthComponent(200))
            addComponent(e, ShellMovementComponent(radius, i))
            addComponent(e, StateMachineComponent('states.ShellItemStateMachine'))
            StateManager.initState(e)
        end
        getEntity('level'):addChild(shell)
        return shell
    end,

    createCamera = function()
        local e = Entity()
        e:emplace('Name', { name = 'level' })
        e:emplace('Position')
        e:emplace('Hierarchy')

        e = Entity()
        e:emplace('Name', { name = 'camera' })
        e:emplace('Position')
        e:emplace('Motion')
        addComponent(e, StateMachineComponent('states.CameraStateMachine'))
        StateManager.initState(e)
        return e
    end,

    shoot = function(power, beamType, pos)
        local force = util.getEntity('force')
        if not force or not force.components.force:isAttached() or power ~= 1 then
            beamType = BeamType.Normal
        end

        shootNormal(power, pos)
        if beamType == BeamType.Ribbon then
            shootRibbon(force, pos)
        elseif beamType == BeamType.Laser then
            shootLaser(pos)
        end

        if power == 1 then
            playSound(Sounds.shoot1)
        else
            playSound(Sounds.shoot2)
        end

        local score = getEntity('score').components.score
        score:setScore(score:getScore() + 1)
    end,

    forceShoot = function()
        local force = getEntity('force')
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

    bitsShoot = function(bits)
        local bitsType = bits.components.bits:getBitsType()
        local pos = bits:getPosition() + vec(4, 0)
        local anim = 'ribbon_small1'
        if bitsType == BitsType.Down then
            anim = 'ribbon_small2'
        end

        local e = Entity()
        e:emplace('Name', { name = 'bits_shot' })
        e:emplace('Position')
        e:emplace('Motion')
        e:emplace('Graphics')
        e:emplace('Hierarchy')
        e:emplace('Collide', { size = vec(32, 12) })
        e:emplace('Animation', { name = 'resources/anims/spaceship.json' })
        e:setAnim(anim, -1)
        e:setVelocity(vec(6, 0))
        e:setPosition(pos)
        addComponent(e, TagComponent('player_shoot'))
        addComponent(e, DamageComponent(getDamageFromPower(1)))
        addComponent(e, StateMachineComponent('states.ShootStateMachine'))
        StateManager.initState(e)
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
        addComponent(e, DamageComponent(0))
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
        e:setPosition(vec(8, 240))
        addComponent(e, StateMachineComponent('states.LivesIndicatorStateMachine'))
        StateManager.initState(e)

        e = Entity()
        e:emplace('Name', { name = '1P' })
        e:emplace('Position')
        e:emplace('Tiles', { name = 'resources/tiles/hud.json' })
        e:setTilesData({ 69, 70, 71, 72 })
        e:setPosition(vec(24, 248))

        e = Entity()
        e:emplace('Name', { name = 'beam_text' })
        e:emplace('Position')
        e:emplace('Tiles', { name = 'resources/tiles/hud.json' })
        e:setTilesData({ 57, 58, 59, 60, 61, 62 })
        e:setPosition(vec(80, 240))

        e = Entity()
        e:emplace('Name', { name = 'beam_level' })
        e:emplace('Position')
        e:emplace('Tiles', { name = 'resources/tiles/hud.json' })
        e:setPosition(vec(128, 240))

        e = Entity()
        e:emplace('Name', { name = 'score' })
        e:emplace('Position')
        e:emplace('Tiles', { name = 'resources/tiles/hud.json' })
        addComponent(e, ScoreComponent(0, vec(104, 248)))

        e = Entity()
        e:emplace('Name', { name = 'hi' })
        e:emplace('Position')
        e:emplace('Tiles', { name = 'resources/tiles/hud.json' })
        e:setPosition(vec(144, 248))
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
        addComponent(e, ScoreComponent(174500, vec(224, 248)))
    end
}

return EntityFactory