local BeamStateMachine = {
    states = {
        MoveState = {
            update = function(e)
                local beam = e.components.beam
                e:setVisible(beam:getPower() > 1)
                beam:update()
            end,
            onKeyDown = function(e, code)
                if code == Keys.Space then
                    e.components.beam:setEnabled(true)
                elseif code == Keys.D1 then
                    e.components.beam:setBeamType(BeamType.Normal)
                elseif code == Keys.D2 then
                    e.components.beam:setBeamType(BeamType.Ribbon)
                end
            end,
            onKeyUp = function(e, code)
                if code == Keys.Space then
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