-- local SDL_MESSAGEBOX_INFORMATION = 0x00000040
-- ShowMessageBox(SDL_MESSAGEBOX_INFORMATION, "Whoops you have to put the CD in your computer")

local stream = Streamer():Open("\\lego\\scripts\\nocd", 0)
if not stream then
	ShowMessageBox(SDL_MESSAGEBOX_INFORMATION, "Failed to open NOCD.si: Streamer failed to load");
	return
end

local ds = MxDSAction:new()

ds:SetAtomId(stream:GetAtom())
ds:SetUnknown24(-1)
ds:SetObjectId(0)
VideoManager():EnableFullScreenMovieWithScale(1, 1)

LEGO:Start(ds)
