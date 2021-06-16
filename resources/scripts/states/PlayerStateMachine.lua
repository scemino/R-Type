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
                    e:setVelocity(vec(e:getVelocity().x, -3))
                end
                if code == Keys.Down then
                    e:setAnim('down', 1)
                    e:setVelocity(vec(e:getVelocity().x, 3))
                end
                if code == Keys.Right then
                    e:setVelocity(vec(3, e:getVelocity().y))
                end
                if code == Keys.Left then
                    e:setVelocity(vec(-3, e:getVelocity().y))
                end
            end,
            onKeyUp = function(e, code)
                if code == Keys.Up then
                    e:setAnim('move', 1)
                    e:setVelocity(vec(e:getVelocity().x, 0))
                end
                if code == Keys.Down then
                    e:setAnim('move', 1)
                    e:setVelocity(vec(e:getVelocity().x, 0))
                end
                if code == Keys.Right then
                    e:setVelocity(vec(0, e:getVelocity().y))
                end
                if code == Keys.Left then
                    e:setVelocity(vec(0, e:getVelocity().y))
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
                    if event.data.entity:getName() == 'enemy_bullet' or
                            event.data.entity:getName() == 'enemy' then
                        return "ExplodingState"
                    end
                end
            end
        },
        ExplodingState = {
            init = function(e)
                e:setVelocity(vec(0, 0))
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