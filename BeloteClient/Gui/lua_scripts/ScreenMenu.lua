-----------------------------------------
-- Script global attributes
-----------------------------------------

local logger = CEGUI.Logger:getSingleton()
local rootWindow = {}

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

	if #screenHistory > 0 then
		rootWindow:getChild(screenHistory[#screenHistory]):setVisible(false)
	end
	rootWindow:getChild(screenName):setVisible(true)
	screenHistory[#screenHistory + 1] = screenName
end

function displayMessage(msgTitle, msg, showOK)
	local errorWnd = rootWindow:getChild("ErrorMsgBox")
	errorWnd:setText(msgTitle)
	errorWnd:getChild("ErrorTxt"):setText(msg)
	errorWnd:getChild("OK"):setVisible(showOK)
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
	
	rootWindow:getChild(screenHistory[#screenHistory]):setVisible(false)
	
	table.remove(screenHistory, #screenHistory)
	rootWindow:getChild(screenHistory[#screenHistory]):setVisible(true)
end

function onConnectionStatusUpdated(args)
	local connStatus = toConnectionStatusEventArgs(args).m_ConnectionStatus
	if connStatus == ClientSocket.CS_Connected then
		Game:getSingleton():LoadGame()
	elseif connStatus == ClientSocket.CS_LobbyFull then
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

function onRulesValid(args)
	rootWindow:getChild("MenuScreenRules/ButtonRulesStart"):enable()
end

function onRulesInvalid(args)
	rootWindow:getChild("MenuScreenRules/ButtonRulesStart"):disable()
end
	
function onRulesStart(args)
	SoundManager:getSingleton():PlayFX(SoundManager.FX_CLICK)

	-- Save player's name
	local game = Game:getSingleton()
	local client = game:GetPlayerSocket()
	local name = rootWindow:getChild("MenuScreenRules/RulesBg/PlayerName"):getText()
	game.m_GameVars.m_PlayerName = CEGUI.String(name)
	
	-- Save play direction
	local pdCBox = CEGUI.toCombobox(rootWindow:getChild("MenuScreenRules/RulesBg/GameRotation"))
	local pdItemIdx = pdCBox:getItemIndex(pdCBox:getSelectedItem())
	game.m_GameVars.m_RuleSet.m_PlayDir = pdItemIdx
	
	-- Save winning score
	local scoreMaxStr = rootWindow:getChild("MenuScreenRules/RulesBg/ScoreMax"):getText()
	game.m_GameVars.m_RuleSet.m_WinningScore = tonumber(scoreMaxStr)
	
	-- Save bots use
	local useBotsCheckBox = CEGUI.toCheckbox(rootWindow:getChild("MenuScreenRules/RulesBg/UseBots"))
	game.m_GameVars.m_RuleSet.m_AllowBots = useBotsCheckBox:isSelected()
	
	-- No worries about joining another host, since only the host gets to the rules def screen
	game:StartServer()
	client.__ClientSocket__:Connect("127.0.0.1", name)
	displayMessage("Patientez", "Connection en cours...", false)
end

	-- Join game screen

function enableJoinGame(args)
	rootWindow:getChild("MenuScreenJoinGame/ButtonJoinDo"):setEnabled(true)
end

function disableJoinGame(args)
	rootWindow:getChild("MenuScreenJoinGame/ButtonJoinDo"):setEnabled(false)
end

function onDoJoinGame(args)
	SoundManager:getSingleton():PlayFX(SoundManager.FX_CLICK)
	
	-- Save game vars
	local game = Game:getSingleton()
	local client = game:GetPlayerSocket()
	
	local pnameBox = rootWindow:getChild("MenuScreenJoinGame/JoinGameBg/PlayerNameClient")
	local hostIpBox = rootWindow:getChild("MenuScreenJoinGame/JoinGameBg/HostIP")
	
	client.__ClientSocket__:Connect(hostIpBox:getText(), pnameBox:getText())
	
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
	local errorWnd = rootWindow:getChild("ErrorMsgBox")
	errorWnd:hide()
	errorWnd:setModalState(false)
end
	
-----------------------------------------
-- Script Entry Point
-----------------------------------------
logger:logEvent("Entering ScreenMenu.lua")
local guiSystem	= CEGUI.System:getSingleton()
local schemeMgr	= CEGUI.SchemeManager:getSingleton()
local winMgr	= CEGUI.WindowManager:getSingleton()
local game		= Game:getSingleton()
local client	= game:GetPlayerSocket()

schemeMgr:createFromFile("OgreTray.scheme");
rootWindow = winMgr:loadLayoutFromFile("ScreenMenu.layout")
guiSystem:setGUISheet(rootWindow)
logger:logEvent("Finished loading menu layout")

-- set default mouse cursor
guiSystem:setDefaultMouseCursor("OgreTrayImages/MouseArrow")
-- set the Tooltip type
guiSystem:setDefaultTooltip("OgreTray/Tooltip")

logger:logEvent("Defaults setup")

-- Setup screens & screen stack
local screensCount = rootWindow:getChildCount()
for screen = 0, screensCount do
	rootWindow:getChildAtIdx(screen):setVisible(false)
end
displayScreen("MenuScreenMain")

logger:logEvent("Screens initialized")

-- Misc. setup
local hostNameBox = CEGUI.toEditbox(rootWindow:getChild("MenuScreenRules/RulesBg/PlayerName"))
hostNameBox:setText("tmp")--game.m_GameVars.m_PlayerName)
local scoreMaxBox = CEGUI.toEditbox(rootWindow:getChild("MenuScreenRules/RulesBg/ScoreMax"))
local clientNameBox = CEGUI.toEditbox(rootWindow:getChild("MenuScreenJoinGame/JoinGameBg/PlayerNameClient"))
local hostIpBox = CEGUI.toEditbox(rootWindow:getChild("MenuScreenJoinGame/JoinGameBg/HostIP"))

local gameRotCB = CEGUI.toCombobox(rootWindow:getChild("MenuScreenRules/RulesBg/GameRotation"))
gameRotCB:addItem(CEGUI.createListboxTextItem("Horaire", 0, nil, false, true))
gameRotCB:addItem(CEGUI.createListboxTextItem("Anti-horaire", 0, nil, false, true))
gameRotCB:setItemSelectState(0, true)

logger:logEvent("Input fields initialized")

-- subscribe required events
	-- Network & errors events
client:subscribeEvent("ConnectionStatusUpdated", "onConnectionStatusUpdated")
game:GetGUIManager():subscribeEvent("ErrorRaised", "onErrorRaised")
	-- Main menu
rootWindow:getChild("MenuScreenMain/ButtonCreateRoom"):subscribeEvent("Clicked", "onHostGame")
rootWindow:getChild("MenuScreenMain/ButtonJoinRoom"):subscribeEvent("Clicked", "onJoinGame")
rootWindow:getChild("MenuScreenMain/ButtonQuit"):subscribeEvent("Clicked", "onQuitGame")
	-- Rules menu
rootWindow:getChild("MenuScreenRules/ButtonRulesStart"):subscribeEvent("Clicked", "onRulesStart")
rootWindow:getChild("MenuScreenRules/ButtonRulesBack"):subscribeEvent("Clicked", "onBack")
hostNameBox:subscribeEvent("ValidEntry", "onRulesValid")
hostNameBox:subscribeEvent("TextInvalidated", "onRulesInvalid")
hostNameBox:subscribeEvent("InvalidEntryAttempted", "onRulesInvalid")
scoreMaxBox:subscribeEvent("ValidEntry", "onRulesValid")
scoreMaxBox:subscribeEvent("TextInvalidated", "onRulesInvalid")
scoreMaxBox:subscribeEvent("InvalidEntryAttempted", "onRulesInvalid")
	-- Join game menu
rootWindow:getChild("MenuScreenJoinGame/ButtonJoinDo"):subscribeEvent("Clicked", "onDoJoinGame")
rootWindow:getChild("MenuScreenJoinGame/ButtonJoinBack"):subscribeEvent("Clicked", "onBack")
clientNameBox:subscribeEvent("ValidEntry", "enableJoinGame")
clientNameBox:subscribeEvent("TextInvalidated", "disableJoinGame")
clientNameBox:subscribeEvent("InvalidEntryAttempted", "disableJoinGame")
hostIpBox:subscribeEvent("ValidEntry", "enableJoinGame")
hostIpBox:subscribeEvent("TextInvalidated", "disableJoinGame")
hostIpBox:subscribeEvent("InvalidEntryAttempted", "disableJoinGame")
	-- Error event
rootWindow:getChild("ErrorMsgBox/OK"):subscribeEvent("Clicked", "onErrorOk")

logger:logEvent("Events suscribed")

-- Finish setup parts which fire events
scoreMaxBox:setText("1000")
clientNameBox:setText("Client")
hostIpBox:setText("127.0.0.1")

logger:logEvent("Input fields initialized #2")