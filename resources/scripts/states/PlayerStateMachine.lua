local util = require 'util'

local PlayerStateMachine = {
    states = {
        IntroState = {},
        MoveState = {
            init = function(e)
                e:setAnim('move', 1)
                e:setVelocity(vec(0, 0))
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
                        if event.data.entity.components.health:isAlive() then
                            return "ExplodingState"
                        end
                    end
                end
            end
        },
        ExplodingState = {
            init = function(e)
                e:setVelocity(vec(-getEntity('camera'):getVelocity().x, 0))
                e:setAnim('explode', 1)
                playSound(Sounds.player_explode)
                e.components.lives:setLives(e.components.lives:getLives() - 1)
                if hasEntity('force') then
                    getEntity('force'):die()
                end
            end,
            anim = function(e, event)
                if event.data.name == 'explode' then
                    -- TODO: here we should check if no more lives in this case go back to main menu
                    e:setPosition(vec(100, 120))
                    -- TODO: here we should go to the last checkpoint
                    setLevelPosition(0)
                    return 'MoveState'
                end
            end
        }
    },
    initialState = 'IntroState'
}
return PlayerStateMachine