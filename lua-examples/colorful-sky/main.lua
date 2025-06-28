local hsx = require('hsx')

local BG_COLOR = {1,0,0}

function CB_Tickle()
    local h, s, v = hsx.rgb2hsv(table.unpack(BG_COLOR))
    h = h + (1/360)

    BG_COLOR[1], BG_COLOR[2], BG_COLOR[3] = hsx.hsv2rgb(h,s,v)

    VideoManager():SetSkyColor(BG_COLOR[1], BG_COLOR[2], BG_COLOR[3])
end
