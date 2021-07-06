local function intro_cutscene()
    tm:startThread(function()
        playSound(Sounds.level_music, -1)
        fadeIn(3)
    end)

    tm:startThread(function()
        local player = getEntity('player')
        delay(2)
        player:setAnim('intro', 1)
        delay(2)
        player:setAnim('intro2', 1)
    end)

    tm:startThread(function()
        local boost = getEntity('boost')
        delay(3)
        boost:setVisible(false)
    end)

    tm:startThread(function()
        local StateMachineComponent = require 'components.StateMachineComponent'
        local player = getEntity('player')
        player:setPosition(vec(-40, 100))
        delay(2)
        moveTo(player, vec(352, 100), 1.5)
        moveTo(player, vec(100, 100), 3)
    end)
end

return { intro = intro_cutscene }