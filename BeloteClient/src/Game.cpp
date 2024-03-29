#include <iostream>

#include <CEGUI.h>

#include <SFML/Network/IpAddress.hpp>
#include <SFML/System/Randomizer.hpp>

#include "TimerPool.h"

#include "Game.h"
#include "SoundManager.h"

Game* Singleton<Game>::ms_Singleton = 0;

Game::Game()
	: m_RenderWindow(0), m_QuitPending(false), m_LoadMenuPending(false), m_LoadGamePending(false)
{
	m_RenderWindow = new sf::RenderWindow(sf::VideoMode(800, 600, 32), "Belote");
	m_RenderWindow->SetFramerateLimit(60);
	m_RenderWindow->ShowMouseCursor(false);

	m_GuiManager.Initialize(m_RenderWindow);

	CEGUI::ImageManager::getSingleton().loadImageset("PlayingCards.imageset");
	CEGUI::AnimationManager::getSingleton().loadAnimationsFromXML("CardHovering.xml");

    m_Server.Init();

	LoadMenu(MR_Normal);
}

Game::~Game()
{
}

void Game::Init()
{
	new Game();
	SoundManager::Init();
	TimerPool::Init();

	sf::Randomizer::SetSeed((unsigned int)time(0));
}

void Game::Run()
{
	sf::Clock clock;
	while (m_RenderWindow->IsOpened())
	{
		HandleEvents();

		if (m_QuitPending)
		{
			// Wait until the click sound has finished playing (if the player clicked on a Quit button).
			if (!SoundManager::getSingleton().IsFXPlaying(SoundManager::FX_CLICK))
				m_RenderWindow->Close();
		}

		if (m_LoadMenuPending)
		{
			DoLoadMenu();
			m_LoadMenuPending = false;
		}

		if (m_LoadGamePending)
		{
			DoLoadGame();
			m_LoadGamePending = false;
		}

		TimerPool::getSingleton().Update();
		m_PlayerSocket.Update();
		m_Server.Update();

		m_RenderWindow->Draw(m_BgSprite);
		m_GuiManager.UpdateAndDraw(clock.GetElapsedTime());
		clock.Reset();
		m_RenderWindow->Display();
	}
}

void Game::HandleEvents()
{
	sf::Event event;

	while (!m_QuitPending && m_RenderWindow->PollEvent(event))
	{
		if (m_GuiManager.HandleEvent(event))
			continue;

		switch (event.Type)
		{
		case sf::Event::Closed:
			Quit();
			break;
				
		default:
			break;
		}
	}
}

void Game::Quit()
{
	m_QuitPending = true;
	
	m_Server.Stop();
	m_PlayerSocket.Disconnect();
}

void Game::DoLoadMenu()
{
	CEGUI::WindowManager::getSingleton().destroyAllWindows();
	m_PlayerSocket.removeAllEvents();

	// Clean up all loaded anims
	CEGUI::AnimationManager &animMgr = CEGUI::AnimationManager::getSingleton();
	for (size_t animIdx = 0; animIdx < animMgr.getNumAnimations(); animIdx++)
	{
		CEGUI::Animation *anim = animMgr.getAnimationAtIdx(animIdx);
		animMgr.destroyAllInstancesOfAnimation(anim);
	}

	CEGUI::System::getSingleton().executeScriptFile("ScreenMenu.lua");

	m_BgImage.LoadFromFile("Gui/ScreenMenu.png");
	m_BgSprite.SetImage(m_BgImage);

	// Display eventual error message
	if (m_MenuReason == MR_Normal)
		return;
	
	ErrorRaisedArgs args;
	switch(m_MenuReason)
	{
	case MR_ConnectionLost:
		args.m_ErrorTxt = GUIManager::ErrorLostConnection;
		break;

	default:
		args.m_ErrorTxt = GUIManager::ErrorUnknown;
		break;
	}
	m_GuiManager.fireEvent(GUIManager::EventErrorRaised, args, GUIManager::EventNamespace);
}

void Game::DoLoadGame()
{
	CEGUI::WindowManager::getSingleton().destroyAllWindows();
	m_PlayerSocket.removeAllEvents();

	CEGUI::System::getSingleton().executeScriptFile("ScreenGame.lua");

	m_BgImage.LoadFromFile("Gui/ScreenGame.png");
	m_BgSprite.SetImage(m_BgImage);
}
