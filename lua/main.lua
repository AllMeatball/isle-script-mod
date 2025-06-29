require('core')
GLOBALS = {
    mouse_x = 0,
    mouse_y = 0,
}

local current_tick = Timer():GetTime()
local new_tick = current_time

local TIME = 0

function CB_Tickle()
    new_tick = Timer():GetTime()
    dt = 1/(new_tick - current_tick)
    current_tick = new_tick

    TIME = TIME + dt
end

function CB_ProcessOneEvent(event)
    if event:GetNotification() == NotificationId.c_notificationKeyPress then
        if event:GetKey() == SDLK_M then
            InvokeAction(ActionType.e_openram, g_nocdSourceName, 0, nil)
        end
        if event:GetKey() == SDLK_C then
            VideoManager():SetSkyColor(math.random(0, 255), math.random(0, 255), math.random(0, 255))
        end
    end

    if event:GetNotification() == NotificationId.c_notificationMouseMove then
        GLOBALS.mouse_x = event:GetX()
        GLOBALS.mouse_y = event:GetY()
--         print(GLOBALS.mouse_x, GLOBALS.mouse_y)
    end

    if event:GetNotification() == NotificationId.c_notificationTransitioned then
        ShowMessageBox(
            SDL_MESSAGEBOX_INFORMATION,
            "Scene Change"
        )
    end
end
