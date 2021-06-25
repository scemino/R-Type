local function solid(pos)
    local tile = map(pos)
    return tile ~= -1
end

local function solid_area(pos, size)
    return solid(vec(pos.x - size.x, pos.y - size.y))
            or solid(vec(pos.x + size.x, pos.y - size.y))
            or solid(vec(pos.x - size.x, pos.y + size.y))
            or solid(vec(pos.x + size.x, pos.y + size.y))
end

local function solid_a(pos, size, vel)
    return solid_area(pos + vel, size)
end

local LaserStateMachine = {
    states = {
        MoveState = {
            update = function(e)
                local camera = Handles[getEntity('camera'):getId()]
                local hb = e:getHitbox()
                local v = e:getVelocity()
                local pos = camera:getPosition() + e:getPosition()
                if solid_a(pos, hb, vec(v.x, 0)) then
                    e.components.laser:flipX()
                elseif solid_a(pos, hb, vec(0, v.y)) then
                    e.components.laser:flipY()
                else
                    e.components.laser:update()
                end
                if e.components.damage:getDamage() == 0 then
                    return 'ExplodingState'
                end
            end,
            hit = function(e, event)
                if event.data.collisionType == 'screen' then
                    e:die()
                --elseif event.data.collisionType == 'tile' then
                --    if event.data.pos.x < e:getPosition().x then
                --        e.components.laser:flipX()
                --    else
                --        e.components.laser:flipY()
                --    endw
                    --return 'ExplodingState'
                end
            end
        },
        ExplodingState = {
            init = function(e)
                e:setVelocity(vec(0, 0))
                playSound(Sounds.shoot_explode2)
                if e.components.damage:getInitialDamage() == 20 then
                    e:setAnim('shoot_explode', 1)
                else
                    e:setAnim('shoot_explode_big', 1)
                end
            end,
            anim = function(e, event)
                if event.data.name == 'shoot_explode' or event.data.name == 'shoot_explode_big' then
                    e:die()
                end
            end
        }
    },
    initialState = 'MoveState'
}
return LaserStateMachine