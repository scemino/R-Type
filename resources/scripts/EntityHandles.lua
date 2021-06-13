-- Thanks to Elias Daler for these functions see it's awesome blog: https://eliasdaler.github.io/game-object-references/
Handles = {}
local memoizedFuncs = {}

-- metatable which does magic
local mt = {}
mt.__index = function(handle, key)
    if not isHandleValid(handle) then
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

local function getWrappedSafeFunction(f)
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
        components = {}
    }

    -- speedy access without __index call
    handle.getName = getWrappedSafeFunction(Entity.getName)

    local id = cppRef:getId()
    setmetatable(handle, mt)
    Handles[id] = handle
    return handle
end

function isHandleValid(handle)
    return handle ~= nil and
            handle.cppRef ~= nil and Handles[handle.cppRef:getId()] ~= nil
end

function onEntityRemoved(id)
    local handle = Handles[id]
    handle.cppRef = nil
    Handles[id] = nil
end

-- components functions
function addComponent(entity, component)
    local handle = Handles[entity:getId()]
    component:setEntity(entity)
    local name = component.class.static.name
    handle.components[name] = component
end

return Handles