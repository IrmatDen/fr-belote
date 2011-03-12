-----------------------------------------
-- Utilities
-----------------------------------------

local CardWidth					= 128
local CardHeight				= 178
local CardPlayablePropertyName	= "Playable" -- the value can be either "1" or "0" (default)
local CardHoverPropertyName		= "AnimInstanceIdx"

local animInstances = { }

function toTextBroadcastedEventArgs(e)
    return tolua.cast(e,"const TextBroadcastedEventArgs")
end

function toPlayerConnectedEventArgs(e)
    return tolua.cast(e,"const PlayerConnectedEventArgs")
end

-- Add a card to the player's current hand
function addCard(cardName)
	local winMgr	= CEGUI.WindowManager:getSingleton()
	local animMgr	= CEGUI.AnimationManager:getSingleton()
	
	local playerHandArea		= winMgr:getWindow("GameArea/PlayerCards")
	local playerHandAreaHeight	= playerHandArea:getHeight().offset
	
	-- Define new windows & animation
	local cardArea = winMgr:createWindow("DefaultWindow", cardName)
	cardArea:setSize(CEGUI.UVector2(CEGUI.UDim(0, CardWidth), CEGUI.UDim(0, playerHandAreaHeight)))
	cardArea:setRiseOnClickEnabled(false)
	cardArea:setUserString(CardPlayablePropertyName, "0")
	cardArea:setUserString(CardHoverPropertyName, #animInstances + 1)
	
	local cardImg = winMgr:createWindow("OgreTray/StaticImage", cardName .. "Img")
	cardImg:setSize(CEGUI.UVector2(CEGUI.UDim(0, CardWidth), CEGUI.UDim(0, CardHeight)))
	cardImg:setYPosition(CEGUI.UDim(1, -CardHeight))
	cardImg:setProperty("FrameEnabled", "False")
	cardImg:setProperty("Image", "PlayingCards/" .. cardName)
	
	local hoverIn	= animMgr:instantiateAnimation("HoverIn")
	local hoverOut	= animMgr:instantiateAnimation("HoverOut")
	hoverIn:setTargetWindow(cardImg)
	hoverOut:setTargetWindow(cardImg)
	table.insert(animInstances, hoverIn)
	table.insert(animInstances, hoverOut)
	
	-- add them to the game area
	cardArea:addChild(cardImg)
	playerHandArea:addChild(cardArea)
	
	-- and allows player to do stuff with the card
	cardArea:subscribeEvent("MouseEntersArea", "onCardHoverIn")
	cardArea:subscribeEvent("MouseLeavesArea", "onCardHoverOut")
	cardImg:subscribeEvent("MouseClick", "onCardSelected")
end

-- Reposition all cards in hand based on how many there are
function rearrangeCards()
	local winMgr			= CEGUI.WindowManager:getSingleton()
	local playerHandArea	= winMgr:getWindow("GameArea/PlayerCards")
	local cardsCount		= playerHandArea:getChildCount() - 1
	local cardVisibleWidth	= CardWidth / 3
	local startX			= (playerHandArea:getWidth().offset / 2)
	startX					= startX	- (cardVisibleWidth * cardsCount)
	
	for cardIdx = 0, cardsCount do
		local card = playerHandArea:getChildAtIdx(cardIdx)
		card:setXPosition(CEGUI.UDim(0, startX))
		
		startX = startX + cardVisibleWidth
	end
end

-- Update playable property on all player's hand's cards
function updatePlayableCards()
	local winMgr			= CEGUI.WindowManager:getSingleton()
	
	-- debug info, will be extracted from somewhere later on.
	winMgr:getWindow("HJ"):setUserString(CardPlayablePropertyName, "1")
	
	local playerHandArea	= winMgr:getWindow("GameArea/PlayerCards")
	local cardsCount		= playerHandArea:getChildCount() - 1
	
	for cardIdx = 0, cardsCount do
		local card = playerHandArea:getChildAtIdx(cardIdx)
		local cardImg = card:getChildAtIdx(0)
		if card:getUserString(CardPlayablePropertyName) == "1" then
			cardImg:setProperty("ImageColours", "tl:FFFFFFFF tr:FFFFFFFF bl:FFFFFFFF br:FFFFFFFF")
		else
			cardImg:setProperty("ImageColours", "tl:FF808080 tr:FF808080 bl:FF808080 br:FF808080")
		end
	end
end

function isCardPlayable(c)
	return tonumber(c:getUserString(CardPlayablePropertyName))
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

function onConnectionStatusUpdated(args)
	local connStatus = toConnectionStatusEventArgs(args).m_ConnectionStatus
	if connStatus == ConnectionStatusEventArgs.CS_Connected then
		Game:getSingleton():LoadGame()
	elseif connStatus == ConnectionStatusEventArgs.CS_LobbyFull then
		print("Lobby full, can't join! This shouldn't happen in-game though, WTF?")
	else -- Disconnected, meaning host unreachable
		print("Connection lost! Get a real error message dude and explain yourself!")
	end
	
	game:LoadMenu()
end

-- Game zone events
function onCardHoverIn(args)
	local window = CEGUI.toWindowEventArgs(args).window
	
	-- Check if card is playable before hovering
	if isCardPlayable(window) == 0 then
		return
	end
	
	local hoverInAnimIdx = window:getUserString(CardHoverPropertyName)
	animInstances[tonumber(hoverInAnimIdx)]:start()
end

function onCardHoverOut(args)
	local window = CEGUI.toWindowEventArgs(args).window
	
	-- Check if card is playable before hovering
	if isCardPlayable(window) == 0 then
		return
	end
	
	local hoverOutAnimIdx = window:getUserString(CardHoverPropertyName) + 1
	animInstances[tonumber(hoverOutAnimIdx)]:start()
end

function onCardSelected(args)
	local window = CEGUI.toWindowEventArgs(args).window:getParent()
	
	-- Check if card is playable before selecting
	if isCardPlayable(window) == 0 then
		return
	end
	
	local cardName = window:getName()
	print ("Selected " .. cardName)
end

-- UI panel events
function onSendChatText(args)
	local we		= CEGUI.toWindowEventArgs(args)
	local game		= Game:getSingleton()
	local client	= game:GetClientSocket()
	
	client:SendChatMessage(we.window:getText())
	we.window:setText("")
end

function onPlayerConnectedStateChange(args)
	local playerCoArgs	= toPlayerConnectedEventArgs(args)
	local text			= "[font='DejaVuSans-8-Bold']" .. playerCoArgs.m_PlayerName
	
	if playerCoArgs.m_Connected then
		text = text .. " s'est connecté"
	else
		text = text .. " s'est déconnecté"
	end
	
	appendTextToChatBox(text)
end

function onTextBroadcasted(args)
	local textArgs	= toTextBroadcastedEventArgs(args)
	local text		= "[font='DejaVuSans-8-Bold']" .. textArgs.m_Teller
	text			= text .. ": [font='DejaVuSans-8']" .. textArgs.m_Message
	
	appendTextToChatBox(text)
end
	
function onQuitTable(args)
	SoundManager:getSingleton():PlayFX(SoundManager.FX_CLICK)

	-- Save player's name
	local game		= Game:getSingleton()
	local client	= game:GetClientSocket()
	
	if game.m_GameVars.m_GameMode == Game.GM_HOST then
		game:StopServer()
	end
	client:Disconnect()
	
	game:LoadMenu()
end
	
-----------------------------------------
-- Script Entry Point
-----------------------------------------
local guiSystem	= CEGUI.System:getSingleton()
local schemeMgr = CEGUI.SchemeManager:getSingleton()
local winMgr	= CEGUI.WindowManager:getSingleton()
local game		= Game:getSingleton()
local client	= game:GetClientSocket()

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
updatePlayableCards()

-- subscribe required events
	-- UI Panel events
local chatTextBox = CEGUI.toEditbox(winMgr:getWindow("UIPanel/ChatBox/Text"))
chatTextBox:subscribeEvent("TextAccepted", "onSendChatText")
winMgr:getWindow("UIPanel/ButtonQuitTable"):subscribeEvent("Clicked", "onQuitTable")
	-- Network events
client:subscribeEvent("ConnectionStatusUpdated", "onConnectionStatusUpdated")
client:subscribeEvent("PlayerConnected", "onPlayerConnectedStateChange")
client:subscribeEvent("PlayerDisconnected", "onPlayerConnectedStateChange")
client:subscribeEvent("TextBroadcasted", "onTextBroadcasted")