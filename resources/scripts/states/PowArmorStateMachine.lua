local function bulletHitEnemy(bullet, enemy)
    local damage = bullet.components.damage:getDamage()
    bullet.components.damage:removeDamage(enemy.components.health:getHealth())
    enemy.components.health:setDamage(damage)
end

local function hitEntities(e, event)
    if event.data.collisionType == 'entities' then
        local name = event.data.entity:getName()
        if name == 'player' then
            return 'ExplodingState'
        elseif name == 'shoot' or name == 'force' or name == 'bits_up' or name == 'bits_dn' then
            bulletHitEnemy(event.data.entity, e)
            if not e.components.health:isAlive() then
                return 'ExplodingState'
            end
        end
    end
end

local function createBits(pos)
    if not hasEntity('bits_up') then
        EntityFactory.createBitsUp(pos)
    elseif not hasEntity('bits_dn') then
        EntityFactory.createBitsDown(pos)
    else
        getEntity('bits_up'):die()
        getEntity('bits_dn'):die()
    end
end

powHeight_2 = vec(0, 11)

local PowArmorStateMachine = {
    states = {
        FlyingState = {
            init = function(e)
                e:setVelocity(vec(-1, 0))
                e:setAnim('flying', 1)
            end,
            update = function(e)
                e.components.pow:update()
                local y = e.components.pow:getY()
                local pos = e:getPosition()
                local nextPos = getEntity('camera'):getPosition() + vec(pos.x, y) + powHeight_2
                local tile = map(nextPos)
                if tile ~= -1 then
                    return 'WalkingState'
                end
                e:setPosition(vec(pos.x, y))
            end,
            hit = function(e, event)
                return hitEntities(e, event)
            end
        },
        WalkingState = {
            init = function(e)
                e:setAnim('walking', -1)
            end,
            update = function(e)
                local nextPos = getEntity('camera'):getPosition() + e:getPosition() + e:getVelocity() + powHeight_2
                local tile = map(nextPos)
                if tile == -1 then
                    return 'JumpingState'
                end
            end,
            hit = function(e, event)
                return hitEntities(e, event)
            end
        },
        JumpingState = {
            init = function(e)
                e:setVelocity(vec(-1, -1))
                e:setAnim('jumping', 1)
            end,
            anim = function(e, _)
                e:setVelocity(vec(-1, 0))
                e.components.pow:setPosition(e:getPosition() + vec(0, -30))
                return 'FlyingState'
            end,
            hit = function(e, event)
                return hitEntities(e, event)
            end
        },
        ExplodingState = {
            init = function(e)
                e:setVelocity(vec(0, 0))
                playSound(Sounds.enemy1_explode)
                e:setAnim('exploding', 1)
                local itemType = e.components.item:getItemType()
                if itemType == ItemType.Bits then
                    createBits(e:getPosition())
                else
                    EntityFactory.createItem(e:getPosition(), itemType)
                end
            end,
            anim = function(e, event)
                if event.data.name == 'exploding' then
                    e:die()
                end
            end
        }
    },
    initialState = 'FlyingState'
}
return PowArmorStateMachine