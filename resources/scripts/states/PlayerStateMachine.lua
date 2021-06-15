local util = require 'util'

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
            end,
            hit = function(e, event)
                if event.data.collisionType == 'tile' then
                    return 'ExplodingState'
                elseif event.data.collisionType == 'screen' then
                    -- clamp player position to screen
                    local pos = e:getPosition()
                    pos = util.clampVector(pos, vec(0, 0), vec(352, 240))
                    e:setPosition(pos)
                elseif event.data.collisionType == 'entities' then
                    print('hit '..event.data.entity:getName())
                    if event.data.entity:getName() ~= 'shoot' then
                        return "ExplodingState"
                    end
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