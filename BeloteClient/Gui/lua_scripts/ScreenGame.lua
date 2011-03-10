-----------------------------------------
-- Utilities
-----------------------------------------

local CardWidth = 128
local CardHeight = 178

function toTextBroadcastedEventArgs(e)
    return tolua.cast(e,"const TextBroadcastedEventArgs")
end

function toPlayerConnectedEventArgs(e)
    return tolua.cast(e,"const PlayerConnectedEventArgs")
end

function addCard(cardName)
	local winMgr = CEGUI.WindowManager:getSingleton()
	
	local card = winMgr:createWindow("OgreTray/PlayingCard", cardName)
	card:setSize(CEGUI.UVector2(CEGUI.UDim(0, CardWidth), CEGUI.UDim(0, CardHeight)))
	card:setXPosition(CEGUI.UDim(0, 0))
	card:setYPosition(CEGUI.UDim(1, -CardHeight))
	card:setProperty("Image", "PlayingCards/" .. cardName)
	card:setZOrderingEnabled(false)
	
	local playerHandArea = winMgr:getWindow("GameArea/PlayerCards")
	playerHandArea:addChild(card)
	
	card:subscribeEvent("MouseClick", "onCardSelected")
end

-- Reposition all cards in hand based on how many there are
function rearrangeCards()
	local winMgr = CEGUI.WindowManager:getSingleton()
	local playerHandArea = winMgr:getWindow("GameArea/PlayerCards")
	local cardsCount = playerHandArea:getChildCount() - 1
	local cardVisibleWidth = CardWidth / 3
	local startX = (playerHandArea:getWidth().offset / 2)
	startX = startX	- (cardVisibleWidth * cardsCount)
	for cardIdx = 0, cardsCount do
		local card = playerHandArea:getChildAtIdx(cardIdx)
		card:setXPosition(CEGUI.UDim(0, startX))
		startX = startX + cardVisibleWidth
	end
end

function appendTextToChatBox(text)
	local winMgr = CEGUI.WindowManager:getSingleton()
	local chatBox = CEGUI.toListbox(winMgr:getWindow("UIPanel/ChatBox/List"))
	local chatItem = LeftWrappedListItem(text)
	chatBox:addItem(chatItem);
	chatBox:ensureItemIsVisible(chatBox:getItemCount());
	tolua.releaseownership(chatItem)
end

-----------------------------------------
-- Start of handler functions
-----------------------------------------

-- Game zone events
function onCardSelected(args)
	print "selected"
end

-- UI panel events
function onSendChatText(args)
	local we = CEGUI.toWindowEventArgs(args)
	local game = Game:getSingleton()
	local client = game:GetClientSocket()
	
	client:SendChatMessage(we.window:getText())
	we.window:setText("")
end

function onPlayerConnectedStateChange(args)
	local playerCoArgs = toPlayerConnectedEventArgs(args)
	local text = "[font='DejaVuSans-8-Bold']" .. playerCoArgs.m_PlayerName
	if playerCoArgs.m_Connected then
		text = text .. " s'est connecté"
	else
		text = text .. " s'est déconnecté"
	end
	appendTextToChatBox(text)
end

function onTextBroadcasted(args)
	local textArgs = toTextBroadcastedEventArgs(args)
	local text = "[font='DejaVuSans-8-Bold']" .. textArgs.m_Teller .. ": [font='DejaVuSans-8']" .. textArgs.m_Message
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

addCard("H7")
addCard("H8")
addCard("H9")
addCard("H10")
addCard("HJ")
addCard("HQ")
addCard("HK")
addCard("H1")
rearrangeCards()

-- subscribe required events
local chatTextBox = CEGUI.toEditbox(winMgr:getWindow("UIPanel/ChatBox/Text"))
chatTextBox:subscribeEvent("TextAccepted", "onSendChatText")
winMgr:getWindow("UIPanel/ButtonQuitTable"):subscribeEvent("Clicked", "onQuitTable")
client:subscribeEvent("PlayerConnected", "onPlayerConnectedStateChange")
client:subscribeEvent("PlayerDisconnected", "onPlayerConnectedStateChange")
client:subscribeEvent("TextBroadcasted", "onTextBroadcasted")