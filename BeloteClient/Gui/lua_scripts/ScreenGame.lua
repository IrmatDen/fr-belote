-----------------------------------------
-- Utilities
-----------------------------------------

local CardWidth					= 96
local CardHeight				= 133.5
local CardPlayablePropertyName	= "Playable" -- the value can be either "1" or "0" (default)
local CardHoverPropertyName		= "AnimInstanceIdx"

local animInstances = { }

local SpeakDisplayPropertyName	= "AnimInstanceIdx"
local speakAnimInstances		= { }
local SpeakAreaNames			= { "LPlayerSpeakArea/Txt", "TPlayerSpeakArea/Txt", "RPlayerSpeakArea/Txt", "" }

local PositionButtonTexts		= { "Sud", "Ouest", "Nord", "Est" }
local PositionButtonNames		= { "ButtonSouth", "ButtonWest", "ButtonNorth", "ButtonEast" }
local myPosition				= 0
local currentPositionning		= { }

local AssetTranslation		= { H = "Coeur", S = "Pique", D = "Carreau", C = "Trèfle" }

local taker = ""
local asset = ""


local PlayedPositions = {	CEGUI.UVector2(CEGUI.UDim(0, 0),				CEGUI.UDim(0.5, -CardHeight / 2)),
							CEGUI.UVector2(CEGUI.UDim(0.5, -CardWidth / 2),	CEGUI.UDim(0, 0)),
							CEGUI.UVector2(CEGUI.UDim(1, -CardWidth),		CEGUI.UDim(0.5, -CardHeight / 2)),
							CEGUI.UVector2(CEGUI.UDim(0.5, -CardWidth / 2),	CEGUI.UDim(1, -CardHeight)),
						}

function toTextBroadcastedEventArgs(e)
    return tolua.cast(e,"const TextBroadcastedEventArgs")
end

function toSystemMessageBroadcastedEventArgs(e)
    return tolua.cast(e,"const SystemMessageBroadcastedEventArgs")
end

function toPlayerConnectedEventArgs(e)
    return tolua.cast(e,"const PlayerConnectedEventArgs")
end

function toCurrentPositioningArgs(e)
    return tolua.cast(e,"const CurrentPositioningArgs")
end

function toCurrentCardsInHandArgs(e)
    return tolua.cast(e,"const CurrentCardsInHandArgs")
end

function toPotentialAssetArgs(e)
    return tolua.cast(e,"const PotentialAssetArgs")
end

function toPlayerDealingArgs(e)
    return tolua.cast(e,"const PlayerDealingArgs")
end

function toPlayerRefusedAssetArgs(e)
    return tolua.cast(e,"const PlayerRefusedAssetArgs")
end

function toPlayerAcceptedAssetArgs(e)
    return tolua.cast(e,"const PlayerAcceptedAssetArgs")
end

function toWaitingPlayArgs(e)
    return tolua.cast(e,"const WaitingPlayArgs")
end

function toPlayedCardArgs(e)
    return tolua.cast(e,"const PlayedCardArgs")
end

function toScoresArgs(e)
    return tolua.cast(e,"const ScoresArgs")
end

function toBeloteAnnouncedArgs(e)
    return tolua.cast(e,"const BeloteAnnouncedArgs")
end

function toContractingTeamResultArgs(e)
    return tolua.cast(e,"const ContractingTeamResultArgs")
end

function toLitigeArgs(e)
    return tolua.cast(e,"const LitigeArgs")
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
	startX					= startX	- (cardVisibleWidth * (cardsCount + 1))
	
	for cardIdx = 0, cardsCount do
		local card = playerHandArea:getChildAtIdx(cardIdx)
		card:setXPosition(CEGUI.UDim(0, startX))
		
		startX = startX + cardVisibleWidth
	end
end

-- Update playable property on all player's hand's cards
function updatePlayableCards()
	local winMgr			= CEGUI.WindowManager:getSingleton()
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

function resetPlayableCards()
	local winMgr			= CEGUI.WindowManager:getSingleton()
	local playerHandArea	= winMgr:getWindow("GameArea/PlayerCards")
	local cardsCount		= playerHandArea:getChildCount() - 1
	
	for cardIdx = 0, cardsCount do
		local card = playerHandArea:getChildAtIdx(cardIdx)
		card:setUserString(CardPlayablePropertyName, "0")
	end
end

function isCardPlayable(c)
	return tonumber(c:getUserString(CardPlayablePropertyName))
end

function setupPlayersName()
	local winMgr				= CEGUI.WindowManager:getSingleton()
	local playerNamesWindows	= { "PlayerLeft", "PlayerPartner", "PlayerRight" }
	for i = 2, 4 do
		local relativePos = myPosition + i - 1
		if relativePos > 4 then
			relativePos = relativePos - 4
		end
		
		winMgr:getWindow(playerNamesWindows[i - 1]):setText(currentPositionning[relativePos])
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

function updateTurnInfoBox(ourScore, theirScore)
	local winMgr	= CEGUI.WindowManager:getSingleton()
	local ti		= winMgr:getWindow("TurnInfo")
	local tiText	= "Preneur : " .. taker
	tiText			= tiText .. "\nAtout : " .. asset
	tiText			= tiText .. "\nEux : " .. theirScore
	tiText			= tiText .. "\nNous : " .. ourScore
	ti:setText(tiText)
end

function deleteAllChildFromWindow(w)
	local winMgr = CEGUI.WindowManager:getSingleton()
	while w:getChildCount() > 0 do
		winMgr:destroyWindow(w:getChildAtIdx(0))
	end
end

function IsPlayerPartOfNSTeam()
	return myPosition == 1 or myPosition == 3
end

function setupAnimForSpeakArea(which)
	local winMgr	= CEGUI.WindowManager:getSingleton()
	local wnd		= winMgr:getWindow(which)
	wnd:setUserString(SpeakDisplayPropertyName, #speakAnimInstances + 1)
	
	local animMgr	= CEGUI.AnimationManager:getSingleton()
	local anim		= animMgr:instantiateAnimation("PlayerSpeak")
	anim:setTargetWindow(wnd)
	table.insert(speakAnimInstances, anim)
end

function playerSays(playerPos, text)
	local relativePos = getRelativePosition(playerPos)
	if relativePos == 4 then return end
	
	local winMgr = CEGUI.WindowManager:getSingleton()
	local window = winMgr:getWindow(SpeakAreaNames[relativePos])
	window:setText(text)
	
	local animIdx = window:getParent():getUserString(SpeakDisplayPropertyName)
	speakAnimInstances[tonumber(animIdx)]:start()
end

function getRelativePosition(playerPos)
	playerPos = playerPos + 1
	local relativePos = playerPos - myPosition
	if relativePos <= 0 then relativePos = 4 + relativePos end
	return relativePos
end

-----------------------------------------
-- Start of handler functions
-----------------------------------------

-- Network events
function onConnectionStatusUpdated(args)
	local game			= Game:getSingleton()
	local connStatus	= toConnectionStatusEventArgs(args).m_ConnectionStatus
	
	if connStatus == ConnectionStatusEventArgs.CS_Disconnected then
		game:LoadMenu(Game.MR_ConnectionLost)
	else
		print("Unexpected connection status: " .. connStatus)
		game:LoadMenu(Game.MR_Unknown)
	end
end

function onCurrentPositioningSent(args)
	local curPosArgs 		= toCurrentPositioningArgs(args)
	local winMgr			= CEGUI.WindowManager:getSingleton()
	local setupBtnsParent	= winMgr:getWindow("GameSetup")
	local playersReady		= 0
	for i = 0, 3 do
		currentPositionning[i + 1] = curPosArgs.m_Pos[i]
		
		local btn = setupBtnsParent:getChild(PositionButtonNames[i + 1])
		if curPosArgs.m_Pos[i] ~= "" then
			btn:setText(PositionButtonTexts[i + 1] .. ": " .. curPosArgs.m_Pos[i])
			btn:disable()
			playersReady = playersReady + 1
		else
			btn:setText(PositionButtonTexts[i + 1])
			btn:enable()
		end
	end
	
	local startBtn = setupBtnsParent:getChild("ButtonStartGame")
	if playersReady == 4 then
		startBtn:enable()
	else
		startBtn:disable()
	end
end

function onGameStarting(args)
	local winMgr = CEGUI.WindowManager:getSingleton()
	winMgr:getWindow("GameSetup"):setVisible(false)
	winMgr:getWindow("GameInProgress"):setVisible(true)
	
	setupPlayersName()
end

function onPlayerDealing(args)
	local dealerArgs	= toPlayerDealingArgs(args)
	local text			= "[font='DejaVuSans-8-Bold']" .. dealerArgs.m_Who .. " distribue"
	
	appendTextToChatBox(text)
	
	local winMgr = CEGUI.WindowManager:getSingleton()
	
	winMgr:getWindow("UIPanel/ButtonSeeLastTurn"):disable()
	
	local winMgr = CEGUI.WindowManager:getSingleton()
	winMgr:getWindow("GameArea/AssetProposal"):setVisible(true)
	winMgr:getWindow("AssetProposalFirstTurn"):setVisible(false)
	winMgr:getWindow("AssetProposalSecondTurn"):setVisible(false)
end

function onCardsReceived(args)
	local handContent	= toCurrentCardsInHandArgs(args)
	local winMgr		= CEGUI.WindowManager:getSingleton()
	
	deleteAllChildFromWindow(winMgr:getWindow("GameArea/PlayerCards"))
	deleteAllChildFromWindow(winMgr:getWindow("GameArea/PlayedCards"))
	deleteAllChildFromWindow(winMgr:getWindow("GameArea/PlayedCardsLastTurn"))
	
	for i = 0, 7 do
		local name = handContent.m_Cards[i]
		if name ~= "" then
			addCard(name)
		end
	end
	
	rearrangeCards()
	updatePlayableCards()
end

function onPotentialAsset(args)
	local winMgr	= CEGUI.WindowManager:getSingleton()
	local asset		= toPotentialAssetArgs(args).m_Card
	winMgr:getWindow("AssetProposalImg"):setProperty("Image", "PlayingCards/" .. asset)
end

function onAskRevealedAsset(args)
	local winMgr = CEGUI.WindowManager:getSingleton()
	winMgr:getWindow("AssetProposalFirstTurn"):setVisible(true)
end

function onAskAnotherAsset(args)
	local winMgr = CEGUI.WindowManager:getSingleton()
	winMgr:getWindow("AssetProposalSecondTurn"):setVisible(true)
	
	-- Build current proposed asset name
	local imgName	= winMgr:getWindow("AssetProposalImg"):getProperty("Image")
	local colour	= imgName:sub(string.len("PlayingCards/") + 1):sub(1, 1)
	local forbiddenAsset = "PlayingCards/Asset" .. colour
	
	-- Extract forbidden asset from all 4 assets images.
	local allAssets		= { "PlayingCards/AssetC", "PlayingCards/AssetD", "PlayingCards/AssetH", "PlayingCards/AssetS" }
	local forbiddenIdx	= 0
	for i = 1, #allAssets do
		if allAssets[i] == forbiddenAsset then
			forbiddenIdx = i
			break
		end
	end
	table.remove(allAssets, forbiddenIdx)
	
	-- Set the potential asset image on the 3 proposal buttons
	for i = 1, #allAssets do
		local assetImg = winMgr:getWindow("AssetImgAlternate" .. i)
		assetImg:setProperty("Image", allAssets[i])
	end
end

function onPlayerAcceptedAsset(args)
	local a = toPlayerAcceptedAssetArgs(args)
	
	playerSays(a.m_PlayerPos, "Je prends à " .. AssetTranslation[a.m_Asset])
	
	if IsPlayerPartOfNSTeam() then
		if args.m_AcceptedByNSTeam then
			taker = "Nous"
		else
			taker = "Eux"
		end
	else
		if args.m_AcceptedByNSTeam then
			taker = "Eux"
		else
			taker = "Nous"
		end
	end
	
	asset = AssetTranslation[a.m_Asset]
	updateTurnInfoBox(0, 0)
end

function onPlayerRefusedAsset(args)
	local pos = toPlayerRefusedAssetArgs(args).m_PlayerPos
	playerSays(pos, "Je passe")
end

function onTurnStarting(args)
	local winMgr = CEGUI.WindowManager:getSingleton()
	winMgr:getWindow("GameArea/AssetProposal"):setVisible(false)
end

function onWaitingPlay(args)
	local possibleCards		= toWaitingPlayArgs(args).m_PossibleCards
	local winMgr			= CEGUI.WindowManager:getSingleton()
	local playerHandArea	= winMgr:getWindow("GameArea/PlayerCards")
	
	resetPlayableCards()
	
	-- and now enable only the ones authorized to play
	for cardIdx = 0, 7 do
		if possibleCards[cardIdx] == "" then
			break
		end
		
		local card = playerHandArea:getChild(possibleCards[cardIdx])
		card:setUserString(CardPlayablePropertyName, "1")
	end
	
	updatePlayableCards()
end

function onPlayedCard(args)
	local pcArgs = toPlayedCardArgs(args)
	
	local winMgr		= CEGUI.WindowManager:getSingleton()
	local playedArea	= winMgr:getWindow("GameArea/PlayedCards")
	
	-- Check if 4 cards are currently in the played area; if so, move 'em to last turn area
	if playedArea:getChildCount() == 4 then
		winMgr:getWindow("UIPanel/ButtonSeeLastTurn"):enable()
		local playedLastTurnArea = winMgr:getWindow("GameArea/PlayedCardsLastTurn")
		deleteAllChildFromWindow(playedLastTurnArea)
		
		while playedArea:getChildCount() > 0 do
			local cardImg = playedArea:getChildAtIdx(0)
			playedArea:removeChild(cardImg)
			playedLastTurnArea:addChild(cardImg)
		end
	end
	
	-- Search relative position to me
	local relativePos = getRelativePosition(pcArgs.m_Player)
	
	-- add card on the table
	local cardImg = winMgr:createWindow("OgreTray/StaticImage", pcArgs.m_Card .. "ImgPlayed")
	cardImg:setSize(CEGUI.UVector2(CEGUI.UDim(0, CardWidth), CEGUI.UDim(0, CardHeight)))
	cardImg:setPosition(PlayedPositions[relativePos])
	cardImg:setProperty("FrameEnabled", "False")
	cardImg:setProperty("Image", "PlayingCards/" .. pcArgs.m_Card)
	
	playedArea:addChild(cardImg)
end

function onCurrentScores(args)
	local scores = toScoresArgs(args)
	local ourScore		= 0
	local theirScore	= 0
	
	if myPosition == 1 or myPosition == 3 then
		ourScore = scores.m_NorthSouthScore
		theirScore = scores.m_WestEastScore
	else
		ourScore = scores.m_WestEastScore
		theirScore = scores.m_NorthSouthScore
	end
	updateTurnInfoBox(ourScore, theirScore)
end

function onTotalScores(args)
	local scores = toScoresArgs(args)
	local ourScore		= 0
	local theirScore	= 0
	
	if myPosition == 1 or myPosition == 3 then
		ourScore = scores.m_NorthSouthScore
		theirScore = scores.m_WestEastScore
	else
		ourScore = scores.m_WestEastScore
		theirScore = scores.m_NorthSouthScore
	end
	
	local winMgr = CEGUI.WindowManager:getSingleton()
	local scoresTable = CEGUI.toMultiColumnList(winMgr:getWindow("UIPanel/Scores/Table"))
	local row = scoresTable:addRow()
	
	local theirScoreItem = CEGUI.createListboxTextItem("[font='DejaVuSans-8']     " .. theirScore)
	local ourScoreItem = CEGUI.createListboxTextItem("[font='DejaVuSans-8']     " .. ourScore)
	scoresTable:setItem(theirScoreItem, 0, row);
	scoresTable:setItem(ourScoreItem, 1, row);
end

function onTextBroadcasted(args)
	local textArgs	= toTextBroadcastedEventArgs(args)
	local text		= "[font='DejaVuSans-8-Bold']" .. textArgs.m_Teller
	text			= text .. ": [font='DejaVuSans-8']" .. textArgs.m_Message
	
	appendTextToChatBox(text)
end

function onSystemMessageBroadcasted(args)
	local sysMsgArgs	= toSystemMessageBroadcastedEventArgs(args)
	local text			= "[font='DejaVuSans-8']" .. sysMsgArgs.m_Message
	
	appendTextToChatBox(text)
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

function onBeloteAnnounced(args)
	local pos = toBeloteAnnouncedArgs(args).m_ByPlayerPos
	playerSays(pos, "Belote !")
end

function onRebeloteAnnounced(args)
	local pos = toBeloteAnnouncedArgs(args).m_ByPlayerPos
	playerSays(pos, "Rebelote !")
end

function onNoAssetTaken(args)
	local text	= "[font='DejaVuSans-8-Bold']Tout le monde a passé"
	appendTextToChatBox(text)
end

function onContractingTeamResult(args)
	local results	= toContractingTeamResultArgs(args)
	local text		= "[font='DejaVuSans-8-Bold']"
	
	if IsPlayerPartOfNSTeam() then
		if results.m_IsNorthSouthTeam then
			text = text .. "Nous avons"
		else
			text = text .. "Ils ont"
		end
	else
		if results.m_IsNorthSouthTeam then
			text = text .. "Ils ont"
		else
			text = text .. "Nous avons"
		end
	end
	
	if results.m_HasWon then
		text = text .. " gagné"
	else
		text = text .. " perdu"
	end
	
	appendTextToChatBox(text)
	
	local winMgr = CEGUI.WindowManager:getSingleton()
	winMgr:getWindow("UIPanel/ButtonSeeLastTurn"):disable()
end

function onLitige(args)
	local litigeScore = toLitigeArgs(args).m_LitigeValue
	local text = "[font='DejaVuSans-8-Bold'] Partie en litige!"
	appendTextToChatBox(text)
	
	local winMgr = CEGUI.WindowManager:getSingleton()
	winMgr:getWindow("UIPanel/ButtonSeeLastTurn"):disable()
end

-- Game zone events
function onChoosePosition(args)
	local window	= CEGUI.toWindowEventArgs(args).window
	local posStr	= window:getName()
	local posName	= posStr:sub(string.len("Button") + 1)
	
	local client	= Game:getSingleton():GetClientSocket()
	client:ChoosePosition(posName)
	
	for i = 1, #PositionButtonNames do
		if posStr ==  PositionButtonNames[i] then
			myPosition = i
		end
	end
end

-- NB: this is when the "Start Game" *button* is clicked, NOT when the game start as per a server's request!
function onStartGameBtn(args)
	Game:getSingleton():GetClientSocket():StartGame()
end

function onAcceptAsset(args)
	local winSrc	= CEGUI.toWindowEventArgs(args).window
	local winMgr	= CEGUI.WindowManager:getSingleton()
	local assetCol	= ""
	
	if winSrc:getName() == "ButtonAcceptAsset" then
		local imgName	= winMgr:getWindow("AssetProposalImg"):getProperty("Image")
		assetCol		= imgName:sub(string.len("PlayingCards/") + 1)
	else
		local imgName	= winSrc:getProperty("Image")
		assetCol		= imgName:sub(string.len("PlayingCards/Asset") + 1)
	end
	
	local client	= Game:getSingleton():GetClientSocket()
	client:AcceptAsset(assetCol)
	
	winMgr:getWindow("AssetProposalFirstTurn"):setVisible(false)
	winMgr:getWindow("AssetProposalSecondTurn"):setVisible(false)
end

function onRefuseAsset(args)
	local client	= Game:getSingleton():GetClientSocket()
	client:RefuseAsset()
	
	local winMgr	= CEGUI.WindowManager:getSingleton()
	winMgr:getWindow("AssetProposalFirstTurn"):setVisible(false)
	winMgr:getWindow("AssetProposalSecondTurn"):setVisible(false)
end

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
	
	-- forbid any more interactions on player's cards.
	-- Calling that later might allow the player to trigger an action, or at least an hover in...
	resetPlayableCards()
	
	local cardName = window:getName()
	
	-- Destroy the window as well as the hover in/out animation instances
	local winMgr = CEGUI.WindowManager:getSingleton()
	winMgr:destroyWindow(window)
	
	local animMgr	= CEGUI.AnimationManager:getSingleton()
	local hoverInAnimIdx = window:getUserString(CardHoverPropertyName)
	local hoverOutAnimIdx = window:getUserString(CardHoverPropertyName) + 1
	animMgr:destroyAnimationInstance(animInstances[tonumber(hoverInAnimIdx)])
	animMgr:destroyAnimationInstance(animInstances[tonumber(hoverOutAnimIdx)])
	
	-- Update the current player's cards
	rearrangeCards()
	updatePlayableCards()
	
	-- and (at least...) notify the server of the player's played card
	local client = Game:getSingleton():GetClientSocket()
	client:PlayCard(cardName)
end

-- UI panel events
function onSendChatText(args)
	local we		= CEGUI.toWindowEventArgs(args)
	local game		= Game:getSingleton()
	local client	= game:GetClientSocket()
	
	client:SendChatMessage(we.window:getText())
	we.window:setText("")
end

function onSeeLastTurn(args)
	local winMgr = CEGUI.WindowManager:getSingleton()
	local lastTurnWnd = winMgr:getWindow("LastTurnWindow")
	lastTurnWnd:show()
	lastTurnWnd:setModalState(true)
end

function onDoneSeeingLastTurn(args)
	local winMgr = CEGUI.WindowManager:getSingleton()
	local lastTurnWnd = winMgr:getWindow("LastTurnWindow")
	lastTurnWnd:hide()
	lastTurnWnd:setModalState(false)
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

-- Setup animations for speaking areas
setupAnimForSpeakArea("LPlayerSpeakArea")
setupAnimForSpeakArea("RPlayerSpeakArea")
setupAnimForSpeakArea("TPlayerSpeakArea")

-- subscribe required events
	-- UI Panel events
local chatTextBox = CEGUI.toEditbox(winMgr:getWindow("UIPanel/ChatBox/Text"))
chatTextBox:subscribeEvent("TextAccepted", "onSendChatText")
winMgr:getWindow("UIPanel/ButtonQuitTable"):subscribeEvent("Clicked", "onQuitTable")
winMgr:getWindow("UIPanel/ButtonSeeLastTurn"):subscribeEvent("Clicked", "onSeeLastTurn")
winMgr:getWindow("ButtonDoneSeeingLastTurn"):subscribeEvent("Clicked", "onDoneSeeingLastTurn")
winMgr:getWindow("ButtonSouth"):subscribeEvent("Clicked", "onChoosePosition")
winMgr:getWindow("ButtonWest"):subscribeEvent("Clicked", "onChoosePosition")
winMgr:getWindow("ButtonNorth"):subscribeEvent("Clicked", "onChoosePosition")
winMgr:getWindow("ButtonEast"):subscribeEvent("Clicked", "onChoosePosition")
winMgr:getWindow("ButtonStartGame"):subscribeEvent("Clicked", "onStartGameBtn")
winMgr:getWindow("ButtonAcceptAsset"):subscribeEvent("Clicked", "onAcceptAsset")
winMgr:getWindow("ButtonRefuseAsset"):subscribeEvent("Clicked", "onRefuseAsset")
winMgr:getWindow("AssetImgAlternate1"):subscribeEvent("MouseClick", "onAcceptAsset")
winMgr:getWindow("AssetImgAlternate2"):subscribeEvent("MouseClick", "onAcceptAsset")
winMgr:getWindow("AssetImgAlternate3"):subscribeEvent("MouseClick", "onAcceptAsset")
winMgr:getWindow("ButtonRefuseAsset2"):subscribeEvent("Clicked", "onRefuseAsset")
	-- Network events
client:subscribeEvent("ConnectionStatusUpdated", "onConnectionStatusUpdated")
client:subscribeEvent("PlayerConnected", "onPlayerConnectedStateChange")
client:subscribeEvent("PlayerDisconnected", "onPlayerConnectedStateChange")
client:subscribeEvent("TextBroadcasted", "onTextBroadcasted")
client:subscribeEvent("SystemMessageBroadcasted", "onSystemMessageBroadcasted")
client:subscribeEvent("CurrentPositioningSent", "onCurrentPositioningSent")
client:subscribeEvent("GameStarting", "onGameStarting")
client:subscribeEvent("PlayerDealing", "onPlayerDealing")
client:subscribeEvent("CardsReceived", "onCardsReceived")
client:subscribeEvent("PotentialAsset", "onPotentialAsset")
client:subscribeEvent("AskRevealedAsset", "onAskRevealedAsset")
client:subscribeEvent("AskAnotherAsset", "onAskAnotherAsset")
client:subscribeEvent("PlayerAcceptedAsset", "onPlayerAcceptedAsset")
client:subscribeEvent("PlayerRefusedAsset", "onPlayerRefusedAsset")
client:subscribeEvent("TurnStarting", "onTurnStarting")
client:subscribeEvent("WaitingPlay", "onWaitingPlay")
client:subscribeEvent("PlayedCard", "onPlayedCard")
client:subscribeEvent("CurrentScores", "onCurrentScores")
client:subscribeEvent("TotalScores", "onTotalScores")
client:subscribeEvent("BeloteAnnounced", "onBeloteAnnounced")
client:subscribeEvent("RebeloteAnnounced", "onRebeloteAnnounced")
client:subscribeEvent("NoAssetTaken", "onNoAssetTaken")
client:subscribeEvent("ContractingTeamResult", "onContractingTeamResult")
client:subscribeEvent("Litige", "onLitige")