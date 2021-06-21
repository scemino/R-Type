local util = require 'util'

local function changeForce(e, code)
    if code == Keys.D1 then
        e.components.force:setForceLevel(1)
    elseif code == Keys.D2 then
        e.components.force:setForceLevel(2)
    elseif code == Keys.D3 then
        e.components.force:setForceLevel(3)
    end
end

local function getForceFrontOffset(e)
    local forceLevel = e.components.force:getForceLevel()
    if forceLevel == 1 then
        return vec(26, 0)
    end
    if forceLevel == 2 then
        return vec(16, 0)
    end
    if forceLevel == 3 then
        return vec(21, 0)
    end
end

local function getForceRearOffset(e)
    local forceLevel = e.components.force:getForceLevel()
    if forceLevel == 1 then
        return vec(-26, 0)
    end
    if forceLevel == 2 then
        return vec(-40, 0)
    end
    if forceLevel == 3 then
        return vec(-30, 0)
    end
end

local function getForceOffset(forceOnTheLeft, e)
    if forceOnTheLeft then
        return getForceRearOffset(e)
    end
    return getForceFrontOffset(e)
end

local function getForceAttachment(forceOnTheLeft)
    if forceOnTheLeft then
        return ForceAttachment.Rear
    end
    return ForceAttachment.Front
end

local AttachedDistance = 10

local ForceStateMachine = {
    states = {
        DetachedState = {
            init = function(e)
                e.components.force:setAttachment(ForceAttachment.None)
            end,
            exit = function(e)
                e:setVelocity(vec(0, 0))
            end,
            update = function(e)
                if not hasEntity('player') then
                    return
                end
                -- detect if player is near the force
                local player = getEntity('player')
                local forcePos = e:getPosition()
                local playerPos = player:getPosition()
                local forceOnTheLeft = forcePos.x < playerPos.x
                playerPos = playerPos + getForceOffset(forceOnTheLeft, e)
                local d = util.distance(playerPos, forcePos)
                if d < AttachedDistance then
                    -- then attach to it
                    local attachment = getForceAttachment(forceOnTheLeft)
                    e.components.force:setAttachment(attachment)
                    return 'AttachedState'
                end

                -- switch force position when player is at the right part of the screen
                local x = 260
                if playerPos.x > 192 then
                    x = 68
                end
                d = util.distance(vec(x, playerPos.y), forcePos)
                if d == 0 then
                    return
                end

                -- move force to the player vertical position
                e:setVelocity(vec((x - forcePos.x) / d, (playerPos.y - forcePos.y) / d))
            end,
            onKeyUp = function(e, code)
                changeForce(e, code)
                -- when detached the force can shoot
                if code == Keys.Z then
                    EntityFactory.forceShoot()
                elseif code == Keys.X then
                    return 'RecallState'
                end
            end
        },
        AttachedState = {
            update = function(e)
                if not hasEntity('player') then
                    return
                end
                local player = getEntity('player')
                local forceOnTheLeft = e.components.force:getAttachment() == ForceAttachment.Rear
                e:setPosition(player:getPosition() + getForceOffset(forceOnTheLeft, e))
            end,
            onKeyUp = function(e, code)
                changeForce(e, code)
                if code == Keys.X then
                    return 'ThrownState'
                end
            end
        },
        ThrownState = {
            init = function(e)
                local dir = vec(4, 0)
                if e.components.force:isRear() then
                    dir = vec(-4, 0)
                end
                e.components.force:setAttachment(ForceAttachment.None)
                e:setVelocity(dir)
            end,
            exit = function(e)
                e:setVelocity(vec(0, 0))
            end,
            hit = function(_, event)
                if event.data.collisionType == 'screen' then
                    return 'DetachedState'
                end
            end,
            onKeyUp = function(e, code)
                changeForce(e, code)
                -- when detached the force can shoot
                if code == Keys.Z then
                    EntityFactory.forceShoot()
                end
            end
        },
        RecallState = {
            exit = function(e)
                e:setVelocity(vec(0, 0))
            end,
            update = function(e)
                if not hasEntity('player') then
                    return
                end
                -- detect if player is near the force
                local forcePos = e:getPosition()
                local player = getEntity('player')
                local playerPos = player:getPosition()
                local forceOnTheLeft = forcePos.x < playerPos.x
                playerPos = playerPos + getForceOffset(forceOnTheLeft, e)
                local d = util.distance(playerPos, forcePos)
                if d < AttachedDistance then
                    -- then attach to it
                    local attachment = getForceAttachment(forceOnTheLeft)
                    e.components.force:setAttachment(attachment)
                    return 'AttachedState'
                end
                -- or move force to the player
                e:setVelocity(vec(2 * (playerPos.x - forcePos.x) / d, 2 * (playerPos.y - forcePos.y) / d))
            end,
            onKeyUp = function(e, code)
                changeForce(e, code)
                -- when detached the force can shoot
                if code == Keys.Z then
                    EntityFactory.forceShoot()
                end
            end
        }
    },
    initialState = 'ThrownState'
}
return ForceStateMachine