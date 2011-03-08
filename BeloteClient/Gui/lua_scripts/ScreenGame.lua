-----------------------------------------
-- Utilities
-----------------------------------------

function toTextBroadcastedEventArgs(e)
    return tolua.cast(e,"const TextBroadcastedEventArgs")
end

function toPlayerConnectedEventArgs(e)
    return tolua.cast(e,"const PlayerConnectedEventArgs")
end

function appendTextToChatBox(text)
	local winMgr = CEGUI.WindowManager:getSingleton()
	local chatBox = CEGUI.toListbox(winMgr:getWindow("UIPanel/ChatBox/List"))
	local chatItem = LeftWrappedListItem(text)
	chatBox:addItem(chatItem);
	chatBox:ensureItemIsVisible(chatBox:getItemCount());
end

-----------------------------------------
-- Start of handler functions
-----------------------------------------

function onSendChatText(args)
	local we = CEGUI.toWindowEventArgs(args)
	local game = Game:getSingleton()
	local client = game:GetClientSocket()
	
	client:SendChatMessage(we.window:getText())
	we.window:setText("")
end

function onPlayerConnectedStateChange(args)
	local playerCoArgs = toPlayerConnectedEventArgs(args)
	local text = "[font='DejaVuSans-10-Bold']" .. playerCoArgs.m_PlayerName
	if playerCoArgs.m_Connected then
		text = text .. " s'est connecté"
	else
		text = text .. " s'est déconnecté"
	end
	appendTextToChatBox(text)
end

function onTextBroadcasted(args)
	local textArgs = toTextBroadcastedEventArgs(args)
	local text = "[font='DejaVuSans-10-Bold']" .. textArgs.m_Teller .. ": [font='DejaVuSans-10']" .. textArgs.m_Message
	appendTextToChatBox(text)
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
local game = Game:getSingleton()
local client = game:GetClientSocket()

schemeMgr:create("OgreTray.scheme");
local root = winMgr:loadWindowLayout("ScreenGame.layout")
guiSystem:setGUISheet(root)

guiSystem:setDefaultMouseCursor("OgreTrayImages/MouseArrow")
guiSystem:setDefaultTooltip("OgreTray/Tooltip")

-- subscribe required events
local chatTextBox = CEGUI.toEditbox(winMgr:getWindow("UIPanel/ChatBox/Text"))
chatTextBox:subscribeEvent("TextAccepted", "onSendChatText")
winMgr:getWindow("UIPanel/ButtonQuitTable"):subscribeEvent("Clicked", "onQuitTable")
client:subscribeEvent("PlayerConnected", "onPlayerConnectedStateChange")
client:subscribeEvent("PlayerDisconnected", "onPlayerConnectedStateChange")
client:subscribeEvent("TextBroadcasted", "onTextBroadcasted")