local StateMachineComponent = require 'components.StateMachineComponent'
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

local function shoot(power, beamType, pos)
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
end

local BeamStateMachine = {
    states = {
        MoveState = {
            update = function(e)
                local beam = e.components.beam
                e:setVisible(beam:getPower() > 1)
                beam:update()
            end,
            onKeyDown = function(e, code)
                if code == Keys.Space then
                    e.components.beam:setEnabled(true)
                elseif code == Keys.D1 then
                    e.components.beam:setBeamType(BeamType.Normal)
                elseif code == Keys.D2 then
                    e.components.beam:setBeamType(BeamType.Ribbon)
                end
            end,
            onKeyUp = function(e, code)
                if code == Keys.Space then
                    e:setVisible(false)
                    local beam = e.components.beam
                    shoot(beam:getPower(), beam:getBeamType(), e:getPosition())
                    beam:setEnabled(false)
                end
            end
        }
    },
    initialState = 'MoveState'
}
return BeamStateMachine