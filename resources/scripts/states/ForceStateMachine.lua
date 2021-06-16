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

local AttachedDistance = 10

local ForceStateMachine = {
    states = {
        DetachedState = {
            init = function(e)
                e.components.force:setAttached(false)
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
                local playerPos = player:getPosition();
                local forcePos = e:getPosition()
                local d = util.distance(playerPos, forcePos)
                if d < AttachedDistance then
                    -- then attach to it
                    return 'AttachedState'
                end

                -- switch force position when player is at the right part of the screen
                local x = 260
                if playerPos.x > 192 then
                    x = 68
                end
                d = util.distance(vec(x, playerPos.y), forcePos)
                if d == 0 then return end

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
            init = function(e)
                e.components.force:setAttached(true)
            end,
            update = function(e)
                if not hasEntity('player') then
                    return
                end
                local player = getEntity('player')
                e:setPosition(player:getPosition() + vec(16, 0))
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
                e.components.force:setAttached(false)
                e:setVelocity(vec(4, 0))
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
                local player = getEntity('player')
                local playerPos = player:getPosition();
                local forcePos = e:getPosition()
                local d = util.distance(playerPos, forcePos)
                if d < AttachedDistance then
                    -- then attach to it
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