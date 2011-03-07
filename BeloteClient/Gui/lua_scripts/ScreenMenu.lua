-----------------------------------------
-- Utilities
-----------------------------------------

local screenHistory = {}
function displayScreen(screenName)
	local winMgr = CEGUI.WindowManager:getSingleton()

	if #screenHistory > 0 then
		winMgr:getWindow(screenHistory[#screenHistory]):setVisible(false)
	end
	winMgr:getWindow(screenName):setVisible(true)
	screenHistory[#screenHistory + 1] = screenName
end

-----------------------------------------
-- Start of handler functions
-----------------------------------------

-- Generic back function to navigate back through the menu screens' history
function onBack(args)
	SoundManager:getSingleton():PlayFX(SoundManager.FX_CLICK)
	
	local winMgr = CEGUI.WindowManager:getSingleton()
	winMgr:getWindow(screenHistory[#screenHistory]):setVisible(false)
	
	table.remove(screenHistory, #screenHistory)
	winMgr:getWindow(screenHistory[#screenHistory]):setVisible(true)
end

	-- Main screen
function onHostGame(args)
	SoundManager:getSingleton():PlayFX(SoundManager.FX_CLICK)
	
	local g = Game:getSingleton()
	g.m_GameVars.m_GameMode = Game.GM_HOST
	
	displayScreen("MenuScreenRules")
end

function onJoinGame(args)
	SoundManager:getSingleton():PlayFX(SoundManager.FX_CLICK)
	Game:getSingleton().m_GameVars.m_GameMode = Game.GM_CLIENT
	
	displayScreen("MenuScreenJoinGame")
end

function onQuitGame(args)
	SoundManager:getSingleton():PlayFX(SoundManager.FX_CLICK)
	Game:getSingleton():Quit()
end

	-- Rules screen
	
function onRulesStart(args)
	SoundManager:getSingleton():PlayFX(SoundManager.FX_CLICK)

	-- Save player's name
	local winMgr = CEGUI.WindowManager:getSingleton()
	local g = Game:getSingleton()
	local pnameBox = winMgr:getWindow("MenuScreenRules/PlayerName")
	GUIManager:LuaStringToCEGUIString(pnameBox:getText(), g.m_GameVars.m_PlayerName)
	
	-- No worries about joining another host, since only the host gets to the rules def screen
	g:StartServer()
	g:JoinGame("127.0.0.1")
	
	print "save game vars & start game"
end

	-- Join game screen

function onIPValid(args)
	local winMgr = CEGUI.WindowManager:getSingleton()
	winMgr:getWindow("MenuScreenJoinGame/ButtonJoinDo"):setEnabled(true)
end

function onIPInvalid(args)
	local winMgr = CEGUI.WindowManager:getSingleton()
	winMgr:getWindow("MenuScreenJoinGame/ButtonJoinDo"):setEnabled(false)
end
	
function onDoJoinGame(args)
	SoundManager:getSingleton():PlayFX(SoundManager.FX_CLICK)
	
	-- Save game vars
	local winMgr = CEGUI.WindowManager:getSingleton()
	local g = Game:getSingleton()
	
	local pnameBox = winMgr:getWindow("MenuScreenJoinGame/PlayerNameClient")
	GUIManager:LuaStringToCEGUIString(pnameBox:getText(), g.m_GameVars.m_PlayerName)
	
	local hostIpBox = winMgr:getWindow("MenuScreenJoinGame/HostIP")
	g.m_GameVars.m_HostIP = hostIpBox:getText()
	
	g:JoinGame(g.m_GameVars.m_HostIP)
	
	print "connect to the game"
end

-----------------------------------------
-- Script Entry Point
-----------------------------------------
local guiSystem = CEGUI.System:getSingleton()
local schemeMgr = CEGUI.SchemeManager:getSingleton()
local winMgr = CEGUI.WindowManager:getSingleton()

schemeMgr:create("OgreTray.scheme");
local root = winMgr:loadWindowLayout("ScreenMenu.layout")
guiSystem:setGUISheet(root)

-- set default mouse cursor
guiSystem:setDefaultMouseCursor("OgreTrayImages/MouseArrow")
-- set the Tooltip type
guiSystem:setDefaultTooltip("OgreTray/Tooltip")

-- Setup screens & screen stack
local screensCount = root:getChildCount()
for screen = 0, screensCount do
	root:getChildAtIdx(screen):setVisible(false)
end
displayScreen("MenuScreenMain")

-- Misc. setup
local gameRotCB = CEGUI.toCombobox(winMgr:getWindow("MenuScreenRules/GameRotation"))
gameRotCB:addItem(CEGUI.createListboxTextItem("Horaire", 0, nil, false, true))
gameRotCB:addItem(CEGUI.createListboxTextItem("Anti-horaire", 0, nil, false, true))
gameRotCB:setItemSelectState(0, true)

local beloteSaveCB = CEGUI.toCombobox(winMgr:getWindow("MenuScreenRules/BeloteSave"))
beloteSaveCB:addItem(CEGUI.createListboxTextItem("Oui", 0, nil, false, true))
beloteSaveCB:addItem(CEGUI.createListboxTextItem("Non", 0, nil, false, true))
beloteSaveCB:setItemSelectState(0, true)

local hostIpBox = CEGUI.toEditbox(winMgr:getWindow("MenuScreenJoinGame/HostIP"))
hostIpBox:setValidationString("(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)")

-- subscribe required events
	-- Main menu
winMgr:getWindow("MenuScreenMain/ButtonCreateRoom"):subscribeEvent("Clicked", "onHostGame")
winMgr:getWindow("MenuScreenMain/ButtonJoinRoom"):subscribeEvent("Clicked", "onJoinGame")
winMgr:getWindow("MenuScreenMain/ButtonQuit"):subscribeEvent("Clicked", "onQuitGame")
	-- Rules menu
winMgr:getWindow("MenuScreenRules/ButtonRulesStart"):subscribeEvent("Clicked", "onRulesStart")
winMgr:getWindow("MenuScreenRules/ButtonRulesBack"):subscribeEvent("Clicked", "onBack")
	-- Join game menu
winMgr:getWindow("MenuScreenJoinGame/ButtonJoinDo"):subscribeEvent("Clicked", "onDoJoinGame")
winMgr:getWindow("MenuScreenJoinGame/ButtonJoinBack"):subscribeEvent("Clicked", "onBack")
hostIpBox:subscribeEvent("ValidEntry", "onIPValid")
hostIpBox:subscribeEvent("TextInvalidated", "onIPInvalid")
hostIpBox:subscribeEvent("InvalidEntryAttempted", "onIPInvalid")