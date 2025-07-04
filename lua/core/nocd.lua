local stream = Streamer():Open("\\lego\\scripts\\nocd", 0)
if not stream then
	ShowMessageBox(MESSAGEBOX_ERROR, "Failed to open NOCD.si: Streamer failed to load");
	return
end

local ds = MxDSAction:new()

ds:SetAtomId(stream:GetAtom())
ds:SetUnknown24(-1)
ds:SetObjectId(0)
VideoManager():EnableFullScreenMovieWithScale(true, true)

LEGO:Start(ds)
