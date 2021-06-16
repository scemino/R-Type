local BeamStateMachine = {
    states = {
        MoveState = {
            update = function(e)
                local beam = e.components.beam
                e:setVisible(beam:getPower() > 1)
                beam:update()
            end,
            onKeyDown = function(e, code)
                if code == Keys.Z then
                    e.components.beam:setEnabled(true)
                elseif code == Keys.D0 then
                    e.components.beam:setBeamType(BeamType.Normal)
                elseif code == Keys.D9 then
                    local force = Handles[getEntity('force'):getId()].components.force
                    if force:getForceLevel() == 1 then
                        force:setForceLevel(2)
                    end
                    e.components.beam:setBeamType(BeamType.Ribbon)
                end
            end,
            onKeyUp = function(e, code)
                if code == Keys.Z then
                    e:setVisible(false)
                    local beam = e.components.beam
                    EntityFactory.shoot(beam:getPower(), beam:getBeamType(), e:getPosition())
                    beam:setEnabled(false)
                end
            end
        }
    },
    initialState = 'MoveState'
}
return BeamStateMachine