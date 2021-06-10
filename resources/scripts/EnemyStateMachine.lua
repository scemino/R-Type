local EnemyStateMachine = {
    states = {
        MoveState = {
            update = function(entity)
                local angle = getComponent(entity,"angle")
                local x = entity:getPosition().x
                local y = math.floor(100+10*math.sin(math.rad(angle)))
                entity:setPosition(vec(x,y))
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
                    entity:remove()
                end
            end
        }
    },
    initialState = "MoveState"
}
return EnemyStateMachine