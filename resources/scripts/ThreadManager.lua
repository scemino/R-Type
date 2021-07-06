-- This code is from Elias Daler (https://eliasdaler.github.io/how-to-implement-action-sequences-and-cutscenes/)
local class = require 'lib.middleclass'

local ThreadManager = class("ThreadManager")

function ThreadManager:initialize()
    self.coroutines = {}
    self.newCoroutines = {}
    self.coroutinesToRemove = {} -- we don't want to remove any coroutines during update
end

function ThreadManager:startThread(f, ...)
    local c = coroutine.create(f)
    local ok, msg = coroutine.resume(c, ...)
    if not ok then
        error("Error in co-routine " .. tostring(c) .. ": " .. msg)
    end

    self:addCoroutine(c)
    return c
end

function ThreadManager:addCoroutine(c)
    if coroutine.status(c) ~= 'dead' then
        if not self.updating then
            table.insert(self.coroutines, c)
        else
            -- insert into newCoroutines table
            -- so the coroutine is not updated twice during usual update
            table.insert(self.newCoroutines, c)
        end
    end
end

function ThreadManager:update()
    self.updating = true
    for _, c in ipairs(self.coroutines) do
        if coroutine.status(c) ~= "dead" then
            local ok, msg = coroutine.resume(c)
            if not ok then
                error("Error in co-routine " .. tostring(c) .. ": " .. msg)
            end
        end
    end
    self.updating = false

    if #self.newCoroutines ~= 0 then
        for _, c in ipairs(self.newCoroutines) do
            table.insert(self.coroutines, c)
        end
        self.newCoroutines = {}
    end

    -- remove finished coroutines
    for i = #self.coroutines, 1, -1 do -- go backwards to remove stuff during iteration
        local c = self.coroutines[i]
        if coroutine.status(c) == 'dead' then
            table.remove(self.coroutines, i)
        end
    end

    -- remove coroutines which were stopped during update
    if #self.coroutinesToRemove ~= 0 then
        for _, c in ipairs(self.coroutinesToRemove) do
            self:stopCoroutine(c)
        end
        self.coroutinesToRemove = {}
    end
end

function ThreadManager:stopCoroutine(co)
    if self.updating then
        table.insert(self.coroutinesToRemove, co)
        return
    end

    local i = 1
    for _, c in ipairs(self.coroutines) do
        if c == co then
            table.remove(self.coroutines, i)
            -- note that coroutine is not stopped here:
            -- it's important to do it, because coroutine might stop itself,
            -- but want to do some things in it afterwards, e.g.
            --     function()
            --          coroutineManager:stopCoroutine(co) -- stop itself
            --          print("STOPPED")
            --     end
            break
        end
        i = i + 1
    end
end

return ThreadManager
