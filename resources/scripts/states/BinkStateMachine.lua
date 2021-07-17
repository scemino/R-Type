local function bulletHitEnemy(bullet, enemy)
    local damage = bullet.components.damage:getDamage()
    bullet.components.damage:removeDamage(enemy.components.health:getHealth())
    enemy.components.health:setDamage(damage)
end

local function hitEntities(e, event)
    if event.data.collisionType == 'entities' then
        local name = event.data.entity:getName()
        print('hit', name)
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

local BinkStateMachine = {
    states = {
        FallingState = {
            init = function(e)
                print('falling')
                e:setVelocity(vec(0, 1))
                e:setAnim('falling', 1)
            end,
            exit = function(e)
                e:setPosition(e:getPosition() + vec(0, -1))
                e:setAnim('falling', 1)
                e:setVelocity(vec(1, 0))
            end,
            hit = function(e, event)
                if event.data.collisionType == 'tile' then
                    return 'WalkingState'
                end
                return hitEntities(e, event)
            end
        },
        WalkingState = {
            init = function(e)
                print('walking')
                e:setAnim('walking', -1)
            end,
            update = function(e)
                local nextPos = getEntity('camera'):getPosition() + e:getPosition() + e:getVelocity() + vec(0, 18)
                local tile = map(nextPos)
                if tile == -1 then
                    return 'JumpingState'
                end
            end,
            hit = function(e, event)
                if event.data.collisionType == 'tile' then
                    return 'StandingState'
                end
                return hitEntities(e, event)
            end
        },
        StandingState = {
            init = function(e)
                print('standing')
                e:setAnim('standing', 1)
                e:setVelocity(vec(0, 0))
            end,
            exit = function(e)
                local flipX = not e:getFlipX()
                local vx = 1
                if flipX then
                    vx = -1
                end
                e:setFlipX(flipX)
                e:setVelocity(vec(vx, 0))
            end,
            anim = function(_, _)
                return 'WalkingState'
            end,
            hit = function(e, event)
                return hitEntities(e, event)
            end
        },
        JumpingState = {
            init = function(e)
                print('jumping')
                e:setAnim('jumping', 1)
            end,
            anim = function(_, _)
                return 'FallingState'
            end,
            hit = function(e, event)
                return hitEntities(e, event)
            end
        },
        ExplodingState = {
            init = function(e)
                e:setVelocity(vec(0, 0))
                playSound(Sounds.enemy1_explode)
                e:setAnim('explode', 1)
            end,
            anim = function(e, event)
                if event.data.name == 'explode' then
                    e:die()
                end
            end
        }
    },
    initialState = 'FallingState'
}
return BinkStateMachine