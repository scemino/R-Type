local function bulletHitEnemy(bullet, enemy)
    local bp = bullet:getPosition()
    local ep = enemy:getPosition()
    local d1 = ep - bp
    local angle = enemy.components.shellItem:getAngle()
    local d2 = vec(math.cos(angle * math.pi / 180), math.cos(angle * math.pi / 180))
    local c = dot(d1, d2)
    local damage = bullet.components.damage:getDamage()
    bullet.components.damage:removeDamage(damage)
    if c > 0 then
        enemy.components.health:setDamage(damage)
    end
end

local ShellItemStateMachine = {
    states = {
        MoveState = {
            update = function(e)
                local shell = getEntity('shell')
                if not shell.components.shell:isAlive() and shell.components.shell:isDead(e.components.shellItem:getIndex()) then
                    return 'ExplodingState'
                elseif e.components.health:isAlive() then
                    e.components.shellItem:update()
                else
                    -- the main shell item has been hot ?
                    if e.components.shellItem:isMain() then
                        shell.components.shell:die()
                    end
                    return 'ExplodingState'
                end
            end,
            hit = function(e, event)
                if event.data.collisionType == 'entities' then
                    if event.data.entity:getName() == 'shoot' then
                        bulletHitEnemy(event.data.entity, e)
                    end
                end
            end
        },
        ExplodingState = {
            init = function(e)
                playSound(Sounds.enemy1_explode)
                local explosion = Entity()
                explosion:emplace('Name', { name = 'explosion' })
                explosion:emplace('Position')
                explosion:emplace('Motion')
                explosion:emplace('Hierarchy')
                explosion:emplace('Graphics')
                explosion:emplace('Animation', { name = 'resources/anims/enemy_shell.json' })
                explosion:setAnim('explode', 1)
                explosion:setPosition(e:getPosition())
                explosion:setZOrder(10)
                e:addChild(explosion)
                e.components.shellItem:die()
            end,
            update = function(e)
                e.components.shellItem:update()
            end,
            hit = function(e, event)
                if event.data.collisionType == 'entities' then
                    local name = event.data.entity:getName()
                    if name == 'shoot' or name == 'force' or name == 'bits_up' or name == 'bits_dn' then
                        bulletHitEnemy(event.data.entity, e)
                    end
                end
            end
        }
    },
    initialState = 'MoveState'
}
return ShellItemStateMachine