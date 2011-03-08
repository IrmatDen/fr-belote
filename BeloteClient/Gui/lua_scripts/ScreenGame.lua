-----------------------------------------
-- Utilities
-----------------------------------------

-----------------------------------------
-- Start of handler functions
-----------------------------------------
	
function onQuitTable(args)
	SoundManager:getSingleton():PlayFX(SoundManager.FX_CLICK)

	-- Save player's name
	local g = Game:getSingleton()
	if g.m_GameVars.m_GameMode == Game.GM_HOST then
		g:StopServer()
		-- Client's disconnection is implied by the server shutting down; no need to repeat.
	else
		g:LeaveGame()
	end
	
	g:LoadMenu()
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

-- set default mouse cursor
guiSystem:setDefaultMouseCursor("OgreTrayImages/MouseArrow")
-- set the Tooltip type
guiSystem:setDefaultTooltip("OgreTray/Tooltip")

-- Setup screens & screen stack
-- local screensCount = root:getChildCount()
-- for screen = 0, screensCount do
	-- root:getChildAtIdx(screen):setVisible(false)
-- end
-- displayScreen("MenuScreenMain")

-- Misc. setup

-- subscribe required events
winMgr:getWindow("UIPanel/ButtonQuitTable"):subscribeEvent("Clicked", "onQuitTable")