GLOBALS = {
    mouse_x = 0,
    mouse_y = 0,
}

-- ShowMessageBox(
--     MESSAGEBOX_INFORMATION,
--     "Scene Change"
-- )

function CB_ProcessOneEvent(event)
    if event:GetNotification() == NotificationId.c_notificationKeyPress then
        if event:GetKey() == 0x0000006d then
--             print('m')
            ShowMessageBox(
                MESSAGEBOX_INFORMATION,
                "Mouse Position X=" ..GLOBALS.mouse_x..", Y="..GLOBALS.mouse_y
            )
            -- LEGO.GetGameState().SwitchArea(Area.e_police)
--             InvokeAction(ActionType.e_openram, g_policeScript, 0)
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
