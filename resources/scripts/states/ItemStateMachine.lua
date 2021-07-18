require 'components.ItemType'
require 'components.BeamType'

local function updateBeam(itemType)
    if itemType == ItemType.Ribbon then
        getEntity('beam').components.beam:setBeamType(BeamType.Ribbon)
    elseif itemType == ItemType.Laser then
        getEntity('beam').components.beam:setBeamType(BeamType.Laser)
    elseif itemType == ItemType.Snake then
        -- TODO
        -- getEntity('beam').components.beam:setBeamType(BeamType.Snake)
    end
end

local function updateForce(itemType)
    if itemType == ItemType.Ribbon or itemType == ItemType.Laser or itemType == ItemType.Snake then
        if not hasEntity('force') then
            EntityFactory.createForce()
        else
            getEntity('force').components.force:addForceLevel()
        end
    end
end

local ItemStateMachine = {
    states = {
        DefaultState = {
            hit = function(e, event)
                if event.data.collisionType == 'entities' then
                    local name = event.data.entity:getName()
                    if name == 'player' then
                        local itemType = e.components.item:getItemType()
                        print('itemType', itemType)
                        updateBeam(itemType)
                        updateForce(itemType)
                        playSound(Sounds.item)
                        e:die()
                    end
                end
            end
        }
    },
    initialState = 'DefaultState'
}
return ItemStateMachine