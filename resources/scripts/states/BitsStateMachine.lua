require 'components.BeamType'

local function getOffset(bitsType)
    if bitsType == BitsType.Up then
        return vec(2, -28)
    end
    return vec(2, 28)
end

local BitsStateMachine = {
    states = {
        InitState = {
            exit = function(e)
                e:setVelocity(vec(0, 0))
            end,
            update = function(e)
                if not hasEntity('player') then
                    return
                end
                local player = getEntity('player')
                local bitsPos = e:getPosition()
                local offset = getOffset(e.components.bits:getBitsType())
                local playerPos = player:getPosition() + offset
                local dx = playerPos.x - bitsPos.x
                local dy = playerPos.y - bitsPos.y
                local d = math.sqrt(dx * dx + dy * dy)
                if d < 1 then
                    e:setVelocity(vec(0, 0))
                    return
                end

                e:setVelocity(vec(1 * dx / 6, 1 * dy / 6))
            end,
            onKeyUp = function(e, code)
                -- bits can shoot only
                if not hasEntity('force') or getEntity('beam').components.beam:getBeamType() ~= BeamType.Ribbon then
                    return
                end
                if code == Keys.Z then
                    EntityFactory.bitsShoot(e)
                end
            end
        }
    },
    initialState = 'InitState'
}
return BitsStateMachine