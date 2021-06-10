local PlayerStateMachine =
{
    states = {
        UpState = {
            init = function(entity)
                local handle = Handles[entity:getId()]
                handle.components.timer:reset()
                entity:setAnim("up", 1)
                local vel = entity:getVelocity()
                vel.y = -3
                entity:setVelocity(vel)
            end,
            update = function(entity)
                local handle = Handles[entity:getId()]
                local timer = handle.components.timer:get()
                if timer == 12 then
                    entity:setAnim("upper", 1)
                else
                    handle.components.timer:update()
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
                local handle = Handles[entity:getId()]
                handle.components.timer:reset()
                entity:setAnim("down", 1)
                local vel = entity:getVelocity()
                vel.y = 3
                entity:setVelocity(vel)
            end,
            update = function(entity)
                local handle = Handles[entity:getId()]
                local timer = handle.components.timer:get()
                if timer == 12 then
                    entity:setAnim("moreDown", 1)
                else
                    handle.components.timer:update()
                end
            end,
            onKeyUp = function(entity, code)
                if code == Keys.Down then
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
                if code == Keys.Up then
                    return "UpState"
                end
                if code == Keys.Down then
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
                    entity:remove()
                end
            end
        }
    },
    initialState = "MoveState"
}
return PlayerStateMachine