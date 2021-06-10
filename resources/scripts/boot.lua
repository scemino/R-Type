-- set up package.path
package.path = package.path .. ";" .. getWorkingDirectory() .. "/resources/scripts/?.lua"

require 'EntityHandles'
require 'StateManager'

-- Keys constants
Keys = {
    Right = 79,
    Left = 80,
    Down = 81,
    Up = 82,
}

-- define enemy
function createEnemy(name)
    print("Create", name)
    local e  = Entity()
    e:emplace("Name", {name=name})
    e:emplace("Position")
    e:emplace("Motion")
    e:emplace("Graphics")
    e:emplace("Collide", {size = vec(32, 12)})
    e:emplace("Animation", {name = "resources/anims/enemy1.json"})
    setComponent(e, "StateMachine", {
        states = {
            MoveState = {
                update = function(entity)
                    local angle = getComponent(entity,"angle")
                    local x = e:getPosition().x
                    local y = math.floor(100+10*math.sin(math.rad(angle)))
                    local p = vec(x,y)
                    e:setPosition(p)
                    setComponent(entity, "angle", angle+5)
                end,
                exit = function(entity)
                    print("MoveState exit")
                end,
                hit = function(entity, event)
                    if event.data.collisionType == "tile" then
                        return "ExplodingState"
                    end
                end
            },
            ExplodingState = {
                init = function(entity)
                    print("BOOM", entity)
                    entity:setAnim("explode", 1)
                end,
                anim = function(entity, event)
                    if event.data.name == "explode" then
                        e:remove()
                    end
                end
            }
        },
        initialState = "MoveState"
    })
    setComponent(e, "angle", 0)
    StateManager.initState(e)
    return e
end

-- define player
function createPlayer()
    print("Create player")
    local e = Entity()
    e:emplace("Name", {name="player"})
    e:emplace("Position")
    e:emplace("Motion")
    e:emplace("Graphics")
    e:emplace("Collide", {size=vec(32, 12)})
    e:emplace("Animation", {name="resources/anims/spaceship.json"})
    setComponent(e, "StateMachine", {
        states = {
            UpState = {
                init = function(entity)
                    setComponent(entity, "timer", 0)
                    entity:setAnim("up", 1)
                    local vel = entity:getVelocity()
                    vel.y = -3
                    entity:setVelocity(vel)
                end,
                update = function(entity)
                    local timer = getComponent(entity,"timer")
                    if timer==12 then
                        entity:setAnim("upper", 1)
                    else
                        setComponent(entity, "timer", timer+1)
                    end
                end,
                onKeyUp = function(entity, code)
                    if code==Keys.Up then
                        return "MoveState"
                    end
                end
            },
            DownState = {
                init = function(entity)
                    setComponent(entity, "timer", 0)
                    entity:setAnim("down", 1)
                    local vel = entity:getVelocity()
                    vel.y = 3
                    entity:setVelocity(vel)
                end,
                update = function(entity)
                    local timer = getComponent(entity,"timer")
                    if timer==12 then
                        entity:setAnim("moreDown", 1)
                    else
                        setComponent(entity, "timer", timer+1)
                    end
                end,
                onKeyUp = function(entity, code)
                    if code==Keys.Down then
                        return "MoveState"
                    end
                end
            },
            MoveState = {
                init= function(entity)
                    entity:setAnim("move", 1)
                    local vel = entity:getVelocity()
                    vel.y = 0
                    entity:setVelocity(vel)
                end,
                onKeyDown = function(entity, code)
                    if code==Keys.Up then
                        return "UpState"                        
                    end
                    if code==Keys.Down then
                        return "DownState"
                    end
                end,
                hit = function(entity, event)
                    if event.data.collisionType == "tile" then
                        return "ExplodingState"
                    end
                end
            },
            ExplodingState = {
                init = function(entity)
                    entity:setAnim("explode", 1)
                end,
                anim = function(entity, event)
                    if event.data.name == "explode" then
                        e:remove()
                    end
                end
            }
        },
        initialState = "MoveState"
    })
    setComponent(e, "timer", 0)
    StateManager.initState(e)
    return e
end

-- main
createPlayer()
createEnemy("enemy1")
createEnemy("enemy2")

function update()
    for key,value in pairs(Handles)
    do
        StateManager.update(value.cppRef)
    end
end

function onEvent(e, event)
    StateManager.onEvent(e, event)
end

function onKeyUp(code)
    for key,value in pairs(Handles)
    do
        StateManager.onKeyUp(value.cppRef, code)
    end
end

function onKeyDown(code)
    for key,value in pairs(Handles)
    do
        StateManager.onKeyDown(value.cppRef, code)
    end
end
