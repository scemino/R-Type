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

return Handles