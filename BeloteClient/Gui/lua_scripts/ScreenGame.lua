-----------------------------------------
-- Utilities
-----------------------------------------

-----------------------------------------
-- Start of handler functions
-----------------------------------------

function onSendChatText(args)
	local we = CEGUI.toWindowEventArgs(args)
	local game = Game:getSingleton()
	local client = game:GetClientSocket()
	
	client:SendChatMessage(we.window:getText())
end
	
function onQuitTable(args)
	SoundManager:getSingleton():PlayFX(SoundManager.FX_CLICK)

	-- Save player's name
	local game = Game:getSingleton()
	local client = game:GetClientSocket()
	if game.m_GameVars.m_GameMode == Game.GM_HOST then
		game:StopServer()
		-- Client's disconnection is implied by the server shutting down; no need to repeat.
	else
		client:Disconnect()
	end
	
	game:LoadMenu()
end
	
-----------------------------------------
-- Script Entry Point
-----------------------------------------
local guiSystem = CEGUI.System:getSingleton()
local schemeMgr = CEGUI.SchemeManager:getSingleton()
local winMgr = CEGUI.WindowManager:getSingleton()

schemeMgr:create("OgreTray.scheme");
local root = winMgr:loadWindowLayout("ScreenGame.layout")
guiSystem:setGUISheet(root)

guiSystem:setDefaultMouseCursor("OgreTrayImages/MouseArrow")
guiSystem:setDefaultTooltip("OgreTray/Tooltip")

-- subscribe required events
local chatTextBox = CEGUI.toEditbox(winMgr:getWindow("UIPanel/ChatBox/Text"))
chatTextBox:subscribeEvent("TextAccepted", "onSendChatText")
winMgr:getWindow("UIPanel/ButtonQuitTable"):subscribeEvent("Clicked", "onQuitTable")