-- Thanks to Elias Daler for these functions see it's awesome blog: https://eliasdaler.github.io/game-object-references/
Handles = {}
local memoizedFuncs = {}

-- metatable which does magic
local mt = {}
mt.__index = function(handle, key)
    if not isHandleValid(handle) then
        print(debug.traceback())
        error("Handle is invalid! (While calling function " .. key .. ")", 2)
    end

    local f = memoizedFuncs[key]
    if not f then
        f = function(handle, ...)
            return Entity[key](handle.cppRef, ...)
        end
        memoizedFuncs[key] = f
    end
    return f
end

local function isAlive(e)
    return e.components.health:isAlive()
end

function createHandle(cppRef)
    local handle = {
        cppRef = cppRef,
        components = {}
    }

    -- speedy access without __index call
    handle.isAlive = isAlive

    local id = cppRef:getId()
    setmetatable(handle, mt)
    Handles[id] = handle
    return handle
end

function isHandleValid(handle)
    return handle ~= nil
end

function onEntityRemoved(id)
    local handle = Handles[id]
    handle.cppRef = nil
    Handles[id] = nil
end

-- components functions
function addComponent(entity, component)
    component:setEntity(entity)
    local name = component.class.static.name
    entity.components[name] = component
end

return Handles