local StateMachineComponent = require 'components.StateMachineComponent'

local function shoot(pos)
    if math.random() > 0.99 then
        local e = Entity()
        e:emplace('Name', { name = 'enemy_bullet' })
        e:emplace('Position')
        e:emplace('Motion')
        e:emplace('Graphics')
        e:emplace('Collide', { size = vec(4, 4) })
        e:emplace('Animation', { name = 'resources/anims/enemy_bullet.json' })
        e:setPosition(pos)
        e:setVelocity(vec(-2, 2))
        addComponent(e, StateMachineComponent('states.EnemyBulletStateMachine'))
        StateManager.initState(e)
    end
end

local function bulletHitEnemy(bullet, enemy)
    local damage = bullet.components.damage:getDamage()
    bullet.components.damage:removeDamage(enemy.components.health:getHealth())
    enemy.components.health:setDamage(damage)
end

local EnemyStateMachine = {
    states = {
        MoveState = {
            update = function(e)
                if e.components.health:isAlive() then
                    e.components.enemyPosition:updatePosition(e)
                    shoot(e:getPosition())
                else
                    return 'ExplodingState'
                end
            end,
            hit = function(e, event)
                if event.data.collisionType == 'entities' then
                    if event.data.entity:getName() == 'player' then
                        return 'ExplodingState'
                    elseif event.data.entity:getName() == 'shoot' then
                        -- TODO: find a way to get this handle from event
                        local handle = Handles[event.data.entity:getId()]
                        bulletHitEnemy(handle, e)
                    end
                end
            end
        },
        ExplodingState = {
            init = function(e)
                print('BOOM', e)
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
    initialState = 'MoveState'
}
return EnemyStateMachine