local StateMachineComponent = require 'StateMachineComponent'
local DamageComponent = require 'DamageComponent'

local function shoot(power, pos)
    print('shoot '..power)
    local e = Entity()
    e:emplace('Name', {name='shoot'})
    e:emplace('Position')
    e:emplace('Motion')
    e:emplace('Graphics')
    e:emplace('Collide', {size=vec(32, 16)})
    e:emplace('Animation', {name='resources/anims/spaceship.json'})
    e:setAnim('shoot'..power, -1)
    e:setPosition(pos+vec(0,4))
    e:setVelocity(vec(12,0))
    addComponent(e, DamageComponent(power))
    addComponent(e, StateMachineComponent('ShootStateMachine'))
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
                local beam = Handles[e:getId()].components.beam
                e:setVisible(beam:getPower()>1)
                beam:update()
            end,
            onKeyDown = function(e, code)
                if code == Keys.Space then
                    Handles[e:getId()].components.beam:setEnabled(true)
                end
            end,
            onKeyUp = function(e, code)
                if code == Keys.Space then
                    e:setVisible(false)
                    local handle = Handles[e:getId()]
                    shoot(handle.components.beam:getPower(), e:getPosition())
                    handle.components.beam:setEnabled(false)
                end
            end
        }
    },
    initialState = 'MoveState'
}
return BeamStateMachine