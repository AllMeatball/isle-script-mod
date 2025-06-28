require('core/sdlkeys')

GLOBALS = {
    mouse_x = 0,
    mouse_y = 0,
}

function CB_ProcessOneEvent(event)
    if event:GetNotification() == NotificationId.c_notificationKeyPress then
        if event:GetKey() == SDLK_M then
            ShowMessageBox(
                MESSAGEBOX_INFORMATION,
                "Mouse Position X=" ..GLOBALS.mouse_x..", Y="..GLOBALS.mouse_y
            )
        end
    end

    if event:GetNotification() == NotificationId.c_notificationMouseMove then
        GLOBALS.mouse_x = event:GetX()
        GLOBALS.mouse_y = event:GetY()
    end
end
