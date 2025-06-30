function UpdateLightPosition(p_increase)
	local lightPosition = tonumber(VariableTable():GetVariable("lightposition"))

    -- Only ever increases by 1 irrespective of p_increase
    if p_increase > 0 then
        lightPosition = lightPosition + 1
        if lightPosition > 5 then
            lightPosition = 5;
        end
    else
        lightPosition = lightPosition - 1
        if lightPosition < 0 then
            lightPosition = 0
        end
    end

    SetLightPosition(lightPosition)

    VariableTable():SetVariable("lightposition", string.format("%d", lightPosition))

    return lightPosition
end

function ConvertHSVToRGB(p_h, p_s, p_v)
	local calc
	local p
	local hueIndex
	local v9
	local v12
	local v13

	local sDbl = p_s

	local rgb = {
		0, 0, 0
	}

	if p_s > 0.5 then
		calc = (1.0 - p_v) * p_s + p_v;
	else
		calc = (p_v + 1.0) * sDbl;
	end

	if calc <= 0.0 then
		rgb[1] = 0.0
		rgb[2] = 0.0
		rgb[3] = 0.0
		return;
	end

	p = p_s * 2.0 - calc
	hueIndex = p_h * 6.0
	v9 = (p_h * 6.0 - hueIndex) * ((calc - p) / calc) * calc
	v12 = p + v9
	v13 = calc - v9

	switch(hueIndex) {
		[0] = function ()
			rgb[1] = calc
			rgb[2] = v12
			rgb[3] = p
		end,
		[1] = function()
			rgb[1] = v13
			rgb[2] = calc
			rgb[3] = p
		end,
		[2] = function()
			rgb[1] = p
			rgb[2] = calc
			rgb[3] = v12
		end,
		[3] = function()
			rgb[1] = p
			rgb[2] = v13
			rgb[3] = calc
		end,
		[4] = function()
			rgb[1] = v12
			rgb[2] = p
			rgb[3] = calc
		end,
		[5] = function()
			rgb[1] = calc
			rgb[2] = p
			rgb[3] = v13
		end,
		[6] = function()
			rgb[1] = calc
			rgb[2] = p
			rgb[3] = v13
		end,
	}

	if switch_func then return rgb end
end

print('Loaded LegoUtils')
