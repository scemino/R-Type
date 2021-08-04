local util = require 'util'

local BeamStateMachine = {
    states = {
        IntroState = {},
        MoveState = {
            update = function(e)
                local beam = e.components.beam
                e:setVisible(beam:getPower() > 1)
                local force = util.entityDo('force', function(f)
                    return f.components.force
                end)
                local ppos = getEntity('player'):getPosition()
                if force and force:isFront() then
                    e:setPosition(ppos + vec(36, 0))
                else
                    e:setPosition(ppos + vec(16, 0))
                end
                beam:update()
            end,
            onKeyDown = function(e, code)
                if code == Keys.Z then
                    e.components.beam:setEnabled(true)
                elseif code == Keys.D0 then
                    e.components.beam:setBeamType(BeamType.Normal)
                elseif code == Keys.D8 then
                    local force = util.entityDo('force', function(f)
                        return f.components.force
                    end)
                    if force and force:getForceLevel() == 1 then
                        force:setForceLevel(2)
                    end
                    e.components.beam:setBeamType(BeamType.Laser)
                elseif code == Keys.D9 then
                    local force = util.entityDo('force', function(f)
                        return f.components.force
                    end)
                    if force and force:getForceLevel() == 1 then
                        force:setForceLevel(2)
                    end
                    e.components.beam:setBeamType(BeamType.Ribbon)
                end
            end,
            onKeyUp = function(e, code)
                if code == Keys.Z then
                    local beam = e.components.beam
                    EntityFactory.shoot(beam:getPower(), beam:getBeamType(), e:getPosition())
                    e:setAnim('beam_shot', 1)
                elseif code == Keys.A then
                    if not hasEntity('force') then
                        EntityFactory.createForce()
                    end
                end
            end,
            anim = function(e, event)
                if event.data.name == 'beam_shot' then
                    local beam = e.components.beam
                    beam:setEnabled(false)
                    e:setVisible(false)
                    e:setAnim('beam', -1)
                end
            end
        }
    },
    initialState = 'IntroState'
}
return BeamStateMachine