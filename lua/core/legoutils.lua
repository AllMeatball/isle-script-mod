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

print('Loaded LegoUtils')
