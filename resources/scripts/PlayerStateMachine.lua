local util = require 'util'

local PlayerStateMachine = {
    states = {
        MoveState = {
            init = function(entity)
                entity:setAnim('move', 1)
                local vel = entity:getVelocity()
                vel.y = 0
                entity:setVelocity(vel)
            end,
            onKeyDown = function(entity, code)
                if code == Keys.Up then
                    entity:setAnim('up', 1)
                    local vel = entity:getVelocity()
                    entity:setVelocity(vec(vel.x, -3))
                end
                if code == Keys.Down then
                    entity:setAnim('down', 1)
                    local vel = entity:getVelocity()
                    entity:setVelocity(vec(vel.x, 3))
                end
                if code == Keys.Right then
                    local vel = entity:getVelocity()
                    entity:setVelocity(vec(3, vel.y))
                end
                if code == Keys.Left then
                    local vel = entity:getVelocity()
                    entity:setVelocity(vec(-3, vel.y))
                end
            end,
            onKeyUp = function(entity, code)
                if code == Keys.Up then
                    entity:setAnim('move', 1)
                    local vel = entity:getVelocity()
                    entity:setVelocity(vec(vel.x, 0))
                end
                if code == Keys.Down then
                    entity:setAnim('move', 1)
                    local vel = entity:getVelocity()
                    entity:setVelocity(vec(vel.x, 0))
                end
                if code == Keys.Right then
                    local vel = entity:getVelocity()
                    entity:setVelocity(vec(0, vel.y))
                end
                if code == Keys.Left then
                    local vel = entity:getVelocity()
                    entity:setVelocity(vec(0, vel.y))
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
            init = function(entity)
                entity:setAnim('explode', 1)
                playSound('resources/audio/rtype-053.wav')
            end,
            anim = function(entity, event)
                if event.data.name == 'explode' then
                    entity:remove()
                end
            end
        }
    },
    initialState = 'MoveState'
}
return PlayerStateMachine