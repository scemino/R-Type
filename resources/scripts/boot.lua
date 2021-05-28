-- factory
function createEnemy(name)
    print("Create ", name)
    local e  = Entity()
    e:emplace("Name", name)
    e:emplace("Position")
    e:emplace("Graphics", "resources/images/r-typesheet1.png", {167, 3, 32, 12});
    return e
end

-- state manager
stateManager = {
    currentState = nil,

    changeState = function(entity, state)
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
        e:setPos(vec(e:getPos().x, math.floor(100+10*math.sin(math.rad(MoveState.a)))))
        if (MoveState.a>=2000) then
            stateManager.changeState(e, ExplodingState)
        end
    end,

    exit = function(entity)
        print("MoveState exit")
    end
}

ExplodingState = {
    init = function(entity)
        print("BOOM")
    end,

    update = function(entity)
    end,

    exit = function(entity)
        print("ExplodingState exit")
    end
}

-- main
enemy = createEnemy("Debug")
enemy:emplace("Animation", "resources/anims/enemy1.json")
stateManager.changeState(enemy,MoveState)

function update()
    currentState.update(enemy)
end
