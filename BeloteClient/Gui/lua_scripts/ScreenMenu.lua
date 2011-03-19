-----------------------------------------
-- Utilities
-----------------------------------------

function toConnectionStatusEventArgs(e)
    return tolua.cast(e,"const ConnectionStatusEventArgs")
end

function toErrorRaisedArgs(e)
    return tolua.cast(e,"const ErrorRaisedArgs")
end

local screenHistory = {}
function displayScreen(screenName)
	local winMgr = CEGUI.WindowManager:getSingleton()

	if #screenHistory > 0 then
		winMgr:getWindow(screenHistory[#screenHistory]):setVisible(false)
	end
	winMgr:getWindow(screenName):setVisible(true)
	screenHistory[#screenHistory + 1] = screenName
end

function displayMessage(msgTitle, msg, showOK)
	local winMgr = CEGUI.WindowManager:getSingleton()
	local errorWnd = winMgr:getWindow("ErrorMsgBox")
	errorWnd:setText(msgTitle)
	errorWnd:getChild("ErrorMsgBox/ErrorTxt"):setText(msg)
	errorWnd:getChild("ErrorMsgBox/OK"):setVisible(showOK)
	errorWnd:show()
	errorWnd:setModalState(true)
end

function displayError(errorString)
	displayMessage("Erreur", errorString, true)
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

function onConnectionStatusUpdated(args)
	local connStatus = toConnectionStatusEventArgs(args).m_ConnectionStatus
	if connStatus == ConnectionStatusEventArgs.CS_Connected then
		Game:getSingleton():LoadGame()
	elseif connStatus == ConnectionStatusEventArgs.CS_LobbyFull then
		displayError("Plus de place sur ce serveur!")
	else -- Disconnected, meaning host unreachable
		displayError("Impossible de se connecter")
	end
end

	-- Main screen
function onHostGame(args)
	SoundManager:getSingleton():PlayFX(SoundManager.FX_CLICK)
	
	Game:getSingleton().m_GameVars.m_GameMode = Game.GM_HOST
	
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

function onHostNameValid(args)
	local winMgr = CEGUI.WindowManager:getSingleton()
	winMgr:getWindow("MenuScreenRules/ButtonRulesStart"):enable()
end

function onHostNameInvalid(args)
	local winMgr = CEGUI.WindowManager:getSingleton()
	winMgr:getWindow("MenuScreenRules/ButtonRulesStart"):disable()
end
	
function onRulesStart(args)
	SoundManager:getSingleton():PlayFX(SoundManager.FX_CLICK)

	-- Save player's name
	local winMgr = CEGUI.WindowManager:getSingleton()
	local game = Game:getSingleton()
	local client = game:GetClientSocket()
	local pnameBox = winMgr:getWindow("MenuScreenRules/PlayerName")
	
	-- No worries about joining another host, since only the host gets to the rules def screen
	game:StartServer()
	client:Connect("127.0.0.1", pnameBox:getText())
end

	-- Join game screen

function enableJoinGame(args)
	local winMgr = CEGUI.WindowManager:getSingleton()
	winMgr:getWindow("MenuScreenJoinGame/ButtonJoinDo"):setEnabled(true)
end

function disableJoinGame(args)
	local winMgr = CEGUI.WindowManager:getSingleton()
	winMgr:getWindow("MenuScreenJoinGame/ButtonJoinDo"):setEnabled(false)
end

function onDoJoinGame(args)
	SoundManager:getSingleton():PlayFX(SoundManager.FX_CLICK)
	
	-- Save game vars
	local winMgr = CEGUI.WindowManager:getSingleton()
	local game = Game:getSingleton()
	local client = game:GetClientSocket()
	
	local pnameBox = winMgr:getWindow("MenuScreenJoinGame/PlayerNameClient")
	local hostIpBox = winMgr:getWindow("MenuScreenJoinGame/HostIP")
	
	client:Connect(hostIpBox:getText(), pnameBox:getText())
	
	displayMessage("Patientez", "Connection en cours...", false)
end

	-- Error

function onErrorRaised(args)
	local errCode = toErrorRaisedArgs(args).m_ErrorTxt
	
	if errCode == GUIManager.ErrorLostConnection then
		displayError("Connexion au serveur perdue.")
	elseif errCode == GUIManager.ErrorUnknown then
		displayError("Erreur inconnue :(")
	end
end
	
function onErrorOk(args)
	local winMgr = CEGUI.WindowManager:getSingleton()
	local errorWnd = winMgr:getWindow("ErrorMsgBox")
	errorWnd:hide()
	errorWnd:setModalState(false)
end
	
-----------------------------------------
-- Script Entry Point
-----------------------------------------
local guiSystem	= CEGUI.System:getSingleton()
local schemeMgr	= CEGUI.SchemeManager:getSingleton()
local winMgr	= CEGUI.WindowManager:getSingleton()
local game		= Game:getSingleton()
local client	= game:GetClientSocket()

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
local hostNameBox = CEGUI.toEditbox(winMgr:getWindow("MenuScreenRules/PlayerName"))
local clientNameBox = CEGUI.toEditbox(winMgr:getWindow("MenuScreenJoinGame/PlayerNameClient"))
local hostIpBox = CEGUI.toEditbox(winMgr:getWindow("MenuScreenJoinGame/HostIP"))

local gameRotCB = CEGUI.toCombobox(winMgr:getWindow("MenuScreenRules/GameRotation"))
gameRotCB:addItem(CEGUI.createListboxTextItem("Horaire", 0, nil, false, true))
gameRotCB:addItem(CEGUI.createListboxTextItem("Anti-horaire", 0, nil, false, true))
gameRotCB:setItemSelectState(0, true)

local beloteSaveCB = CEGUI.toCombobox(winMgr:getWindow("MenuScreenRules/BeloteSave"))
beloteSaveCB:addItem(CEGUI.createListboxTextItem("Oui", 0, nil, false, true))
beloteSaveCB:addItem(CEGUI.createListboxTextItem("Non", 0, nil, false, true))
beloteSaveCB:setItemSelectState(0, true)

-- subscribe required events
	-- Network & errors events
client:subscribeEvent("ConnectionStatusUpdated", "onConnectionStatusUpdated")
game:GetGUIManager():subscribeEvent("ErrorRaised", "onErrorRaised")
	-- Main menu
winMgr:getWindow("MenuScreenMain/ButtonCreateRoom"):subscribeEvent("Clicked", "onHostGame")
winMgr:getWindow("MenuScreenMain/ButtonJoinRoom"):subscribeEvent("Clicked", "onJoinGame")
winMgr:getWindow("MenuScreenMain/ButtonQuit"):subscribeEvent("Clicked", "onQuitGame")
	-- Rules menu
winMgr:getWindow("MenuScreenRules/ButtonRulesStart"):subscribeEvent("Clicked", "onRulesStart")
winMgr:getWindow("MenuScreenRules/ButtonRulesBack"):subscribeEvent("Clicked", "onBack")
hostNameBox:subscribeEvent("ValidEntry", "onHostNameValid")
hostNameBox:subscribeEvent("TextInvalidated", "onHostNameInvalid")
hostNameBox:subscribeEvent("InvalidEntryAttempted", "onHostNameInvalid")
	-- Join game menu
winMgr:getWindow("MenuScreenJoinGame/ButtonJoinDo"):subscribeEvent("Clicked", "onDoJoinGame")
winMgr:getWindow("MenuScreenJoinGame/ButtonJoinBack"):subscribeEvent("Clicked", "onBack")
clientNameBox:subscribeEvent("ValidEntry", "enableJoinGame")
clientNameBox:subscribeEvent("TextInvalidated", "disableJoinGame")
clientNameBox:subscribeEvent("InvalidEntryAttempted", "disableJoinGame")
hostIpBox:subscribeEvent("ValidEntry", "enableJoinGame")
hostIpBox:subscribeEvent("TextInvalidated", "disableJoinGame")
hostIpBox:subscribeEvent("InvalidEntryAttempted", "disableJoinGame")
	-- Error event
winMgr:getWindow("ErrorMsgBox/OK"):subscribeEvent("Clicked", "onErrorOk")

-- Finish setup parts which fire events
hostNameBox:setText("Host")
clientNameBox:setText("Client")
hostIpBox:setText("127.0.0.1")