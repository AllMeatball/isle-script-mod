local g_nextCharacter = 0

-- GLOBAL: LEGO1 0x100f66c0
local g_debugPassword = {
	-- "OGEL"
	SDLK_O,
	SDLK_G,
	SDLK_E,
	SDLK_L,
	false,
}

local g_currentInputIndex, g_currentInput = next(g_debugPassword, nil)

local function OGEL_keys(nav, p_param)
	if not g_currentInput then
		print('OGEL KEY')
	else
		if g_currentInput == p_param:GetKey() then
			g_currentInputIndex, g_currentInput = next(g_debugPassword, g_currentInputIndex)
		else
			g_currentInputIndex, g_currentInput = next(g_debugPassword, nil)
		end
	end
end

local function CycleThroughCharacters(key) -- Cycles through characters and puts them in front of you
	if g_nextCharacter == -1 then
		g_nextCharacter = 0
	else
		CharacterManager():ReleaseActor( CharacterManager():GetActorName(g_nextCharacter) )

		if key == SDLK_KP_PLUS then
			g_nextCharacter = (g_nextCharacter + 1)
			if g_nextCharacter >= CharacterManager():GetNumActors() then
				g_nextCharacter = 0;
			end
		else
			g_nextCharacter = (g_nextCharacter - 1)
			if g_nextCharacter < 0 then
				g_nextCharacter = CharacterManager():GetNumActors() - 1;
			end
		end
	end

	local roi = CharacterManager():GetActorROI(CharacterManager():GetActorName(g_nextCharacter), true) -- LegoROI*
	if roi ~= nil then
		local mat
		local viewRoi = VideoManager():GetViewROI()

		local position = viewRoi:GetWorldPosition()
		local direction = viewRoi:GetWorldDirection()
		local up = viewRoi:GetWorldUp()

		CalcLocalTransform(position, direction, up, mat)
		mat:TranslateBy(direction[0] * 2.0, direction[1] - 1.0, direction[2] * 2.0)

		roi:UpdateTransformationRelativeToParent(mat)
	end
end

function CB_NavController_Notify(self, p_param)
	if p_param:GetNotification() ~= NotificationId.c_notificationKeyPress then return end

	self:Unk0x5dHack(true)
	local key = p_param:GetKey()

	switch(key) {
		[SDLK_PAUSE] = function() -- Pause game
			if LEGO:IsPaused() then
				LEGO:Resume()
			else
				LEGO:Pause()
			end
		end,

		[SDLK_ESCAPE] = function() -- Return to infocenter
			local currentWorld = CurrentWorld()
			if currentWorld == nil then return end

			local state = GetInfocenterState()
			assert(state)

			if state ~= nil and state.m_unk0x74 ~= 8 and currentWorld:Escape() then
				BackgroundAudioManager():Stop()
				TransitionManager():StartTransition(TransitionType.e_mosaic, 50, false, false)
				state.m_unk0x74 = 8
			end
		end,

		[SDLK_SPACE] = function() -- Interrupt/end animations or free navigation
			AnimationManager():FUN_10061010(true)
		end,

		[SDLK_F12] = function() -- Saves the game
			local state = GetInfocenterState()
			if state and state:HasRegistered() then
				GameState():Save(0)
			end
		end,

		-- Cycles through characters and puts them in front of you
		[SDLK_KP_PLUS] = CycleThroughCharacters,
		[SDLK_KP_MINUS] = CycleThroughCharacters,

		[SDLK_Z] = function() -- Make nearby plants "dance"
			local omni = LEGO

			if omni:GetCurrentWorld() ~= nil and omni:GetCurrentWorld():GetWorldId() == WorldId.e_act1 then
				local videoMgr = VideoManager()
				local roi = videoMgr:GetViewROI() -- ViewROI*
				local view = videoMgr:Get3DManager():GetLego3DView():GetViewManager() -- ViewManager*
				local plantMgr = LEGO:GetPlantManager()
				local viewPosition = Mx3DPointFloat:new(roi:GetWorldPosition()) -- Mx3DPointFloat
				local numPlants = plantMgr:GetNumPlants()

				for i=0,numPlants do
					local entity = plantMgr:CreatePlant(i, nil, WorldId.e_act1) -- LegoEntity*

					-- LegoEntity::c_altBit1
					if entity ~= nil and not entity:GetUnknown0x10IsSet(LegoEntity_c_altBit1) then
						local roi = entity:GetROI() -- LegoROI*

						if roi ~= nil and roi:GetVisibility() then
							local box = roi:GetWorldBoundingBox() -- const BoundingBox&

							if view:IsBoundingBoxInFrustum(box) then
								local roiPosition = Mx3DPointFloat:new(roi:GetWorldPosition()) -- Mx3DPointFloat
								roiPosition = roiPosition - viewPosition

								if roiPosition.LenSquared() < 2000.0 or roi:GetLodLevel() > 0 then
									entity:ClickAnimation()
								end
							end
						end
					end
				end
			end
		end,

		[CASE_DEFAULT] = function (key)
			OGEL_keys(nav, p_param)
		end,
	}

end
