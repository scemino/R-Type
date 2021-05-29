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
        isValid = true
    }

    -- speedy access without __index call
    --handle.getName = getWrappedSafeFunction(Entity.getName)

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

-- factory
function createEnemy(name)
    print("Create ", name)
    local e  = Entity()
    e:emplace("Name", name)
    e:emplace("Position")
    e:emplace("Graphics", "resources/images/r-typesheet1.png", {167, 3, 32, 12})
    e:emplace("Collide", {32, 12})
    e:emplace("Animation", "resources/anims/enemy1.json")
    return e
end

-- state manager
stateManager = {
    currentState = nil,

    changeState = function(entity, state)
        if currentState==state then
            return
        end
        if currentState then
            currentState.exit(entity)
        end
        currentState = state
        currentState.init(entity)
    end
}

-- define enemy states
MoveState = {
    a = 0,

    init = function(entity)
        print("MoveState init ", MoveState.a)
    end,

    update = function(e)
        MoveState.a=MoveState.a+5
        --e:setPos(vec(e:getPos().x, math.floor(100+10*math.sin(math.rad(MoveState.a)))))
        -- if (MoveState.a>=2000) then
        --     stateManager.changeState(e, ExplodingState)
        -- end
    end,

    exit = function(entity)
        print("MoveState exit")
    end
}

ExplodingState = {
    init = function(entity)
        print("BOOM", entity)
        entity:setAnim("explode", 1)
    end,

    update = function(entity)
    end,

    exit = function(entity)
        print("ExplodingState exit")
    end
}

-- main
enemy1 = createEnemy("enemy1")
stateManager.changeState(enemy1, MoveState)

function update()
    currentState.update(enemy1)
    currentState.update(enemy2)
end

function onEvent(e, event)
    if event.type == "hit" and event.data.collisionType == "tile" then
        stateManager.changeState(e, ExplodingState)
    end
    if event.type == "anim" and event.data.name=="explode" then
        print("Anim ended:", event.data.name)
        e:remove()
    end
end
