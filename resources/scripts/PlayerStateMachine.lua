local util = require 'util'
local StateMachineComponent = require 'StateMachineComponent'

local function shoot(power, pos)
    local e = Entity()
    e:emplace('Name', {name='shoot'})
    e:emplace('Position')
    e:emplace('Motion')
    e:emplace('Graphics')
    e:emplace('Collide', {size=vec(32, 16)})
    e:emplace('Animation', {name='resources/anims/spaceship.json'})
    e:setAnim('shoot'..power, -1)
    e:setPosition(pos+vec(32,4))
    e:setVelocity(vec(6,0))
    addComponent(e, StateMachineComponent('ShootStateMachine'))
    StateManager.initState(e)
    if power == 1 then
        playSound(Sounds.shoot1)
    else
        playSound(Sounds.shoot2)
    end
end

local PlayerStateMachine = {
    states = {
        MoveState = {
            init = function(e)
                e:setAnim('move', 1)
                local vel = e:getVelocity()
                vel.y = 0
                e:setVelocity(vel)
            end,
            onKeyDown = function(e, code)
                if code == Keys.Up then
                    e:setAnim('up', 1)
                    local vel = e:getVelocity()
                    e:setVelocity(vec(vel.x, -3))
                end
                if code == Keys.Down then
                    e:setAnim('down', 1)
                    local vel = e:getVelocity()
                    e:setVelocity(vec(vel.x, 3))
                end
                if code == Keys.Right then
                    local vel = e:getVelocity()
                    e:setVelocity(vec(3, vel.y))
                end
                if code == Keys.Left then
                    local vel = e:getVelocity()
                    e:setVelocity(vec(-3, vel.y))
                end
            end,
            onKeyUp = function(e, code)
                if code == Keys.Up then
                    e:setAnim('move', 1)
                    local vel = e:getVelocity()
                    e:setVelocity(vec(vel.x, 0))
                end
                if code == Keys.Down then
                    e:setAnim('move', 1)
                    local vel = e:getVelocity()
                    e:setVelocity(vec(vel.x, 0))
                end
                if code == Keys.Right then
                    local vel = e:getVelocity()
                    e:setVelocity(vec(0, vel.y))
                end
                if code == Keys.Left then
                    local vel = e:getVelocity()
                    e:setVelocity(vec(0, vel.y))
                end
                if code == Keys.Space then
                    shoot(1, e:getPosition())
                end
            end,
            hit = function(e, event)
                if event.data.collisionType == 'tile' then
                    return 'ExplodingState'
                else
                    -- clamp player position to screen
                    local pos = e:getPosition()
                    pos = util.clampVector(pos, vec(0, 0), vec(352, 240))
                    e:setPosition(pos)
                end
            end
        },
        ExplodingState = {
            init = function(e)
                e:setVelocity(vec(0,0))
                e:setAnim('explode', 1)
                playSound(Sounds.player_explode)
            end,
            anim = function(e, event)
                if event.data.name == 'explode' then
                    e:die()
                end
            end
        }
    },
    initialState = 'MoveState'
}
return PlayerStateMachine