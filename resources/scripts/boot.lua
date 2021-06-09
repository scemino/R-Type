-- Thanks to Elias Daler for these functions see it's awesome blog: https://eliasdaler.github.io/game-object-references/
Handles = {}
local memoizedFuncs = {}

-- metatable which does magic
local mt = {}
mt.__index = function(handle, key)
    if not handle.isValid then
        print(debug.traceback())
        error("Error: handle is not valid!", 2)
    end

    local f = memoizedFuncs[key]
    if not f then
        f = function(handle, ...) return Entity[key](handle.cppRef, ...) end
        memoizedFuncs[key] = f
    end
    return f
end

function getWrappedSafeFunction(f)
    return function(handle, ...)
            if not handle.isValid then
                print(debug.traceback())
                error("Error: handle is not valid!", 2)
            end
            return f(handle.cppRef, ...)
        end
end

function createHandle(cppRef)
    local handle = {
        cppRef = cppRef,
        isValid = true,
        components = {}
    }

    -- speedy access without __index call
    handle.getName = getWrappedSafeFunction(Entity.getName)

    local id = cppRef:getId()
    setmetatable(handle, mt)
    Handles[id] = handle
    return handle
end

function onEntityRemoved(cppRef)
    local handle = Handles[cppRef:getId()];
    handle.cppRef = nil
    handle.isValid = false
    Handles[cppRef:getId()] = nil
end

-- components functions
function setComponent(e, name, data)
    Handles[e:getId()].components[name]=data
end

function getComponent(e, name)
    local handle = Handles[e:getId()]
    if handle then
        return handle.components[name]
    end
end

-- Keys constants
Keys = {
    Right = 79,
    Left = 80,
    Down = 81,
    Up = 82,
}

-- state manager
StateManager = {
    currentState = nil,

    changeState = function(entity, state)
        local sm = getComponent(entity, "StateMachine")
        if sm.currentState==state then
            return
        end
        if sm.currentState then
            local state = sm.states[sm.currentState]
            if state.exit then
                state.exit(entity)
            end
        end
        sm.currentState = state
        local state = sm.states[sm.currentState]
        if state.init then
            state.init(entity)
        end
    end,

    initState = function(entity)
        local sm = getComponent(entity, "StateMachine")
        StateManager.changeState(entity, sm.initialState)
        sm.currentState = sm.initialState
    end,

    update = function(entity)
        local sm = getComponent(entity, "StateMachine")
        local state = sm.states[sm.currentState]
        if state.update then
            local nextState = state.update(entity)
            if nextState then
                StateManager.changeState(entity, nextState)
            end
        end
    end,

    onEvent = function(entity, event)
        local sm = getComponent(entity, "StateMachine")
        if not sm then
            return nil
        end
        local callback = sm.states[sm.currentState][event.type]
        if callback then
            local nextState = callback(entity, event)
            if nextState then
                StateManager.changeState(entity, nextState)
            end
        end
    end,

    onKeyUp = function(entity, code)
        local sm = getComponent(entity, "StateMachine")
        local callback = sm.states[sm.currentState]["onKeyUp"]
        if callback then
            local nextState = callback(entity, code)
            if nextState then
                StateManager.changeState(entity, nextState)
            end
        end
    end,

    onKeyDown = function(entity, code)
        local sm = getComponent(entity, "StateMachine")
        local callback = sm.states[sm.currentState]["onKeyDown"]
        if callback then
            local nextState = callback(entity, code)
            if nextState then
                StateManager.changeState(entity, nextState)
            end
        end
    end,
}

-- define enemy states
function createEnemy(name)
    print("Create", name)
    local e  = Entity()
    e:emplace("Name", name)
    e:emplace("Position")
    e:emplace("Motion")
    e:emplace("Graphics")
    e:emplace("Collide", {32, 12})
    e:emplace("Animation", "resources/anims/enemy1.json")
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

-- create player
function createPlayer()
    print("Create player")
    local e  = Entity()
    e:emplace("Name", "player")
    e:emplace("Position")
    e:emplace("Motion")
    e:emplace("Graphics")
    e:emplace("Collide", {32, 12})
    e:emplace("Animation", "resources/anims/spaceship.json")
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
