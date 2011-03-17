#include <iostream>

#include <CEGUI.h>

#include <SFML/Network/IpAddress.hpp>
#include <SFML/System/Randomizer.hpp>

#include "Game.h"
#include "SoundManager.h"

Game* Singleton<Game>::ms_Singleton = 0;

Game::Game()
	: m_RenderWindow(0), m_QuitPending(false), m_LoadMenuPending(false), m_LoadGamePending(false),
	m_ServerThread(&Server::Start, &m_Server)
{
	m_RenderWindow = new sf::RenderWindow(sf::VideoMode(800, 600, 32), "Belote");
	m_RenderWindow->SetFramerateLimit(60);
	m_RenderWindow->ShowMouseCursor(false);

	m_GuiManager.Initialize(m_RenderWindow);

	CEGUI::ImageManager::getSingleton().loadImageset("PlayingCards.imageset");
	CEGUI::AnimationManager::getSingleton().loadAnimationsFromXML("CardHovering.xml");

	LoadMenu();
}

Game::~Game()
{
}

void Game::Init()
{
	new Game();
	SoundManager::Init();

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

		m_ClientSocket.Update();

		m_RenderWindow->Draw(m_BgSprite);
		m_GuiManager.UpdateAndDraw(clock.GetElapsedTime());
		clock.Reset();
		m_RenderWindow->Display();
	}
}

void Game::HandleEvents()
{
	sf::Event event;

	while (!m_QuitPending && m_RenderWindow->GetEvent(event))
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
	m_ClientSocket.Disconnect();
	m_ClientSocket.Wait();
}

void Game::DoLoadMenu()
{
	CEGUI::WindowManager::getSingleton().destroyAllWindows();
	m_ClientSocket.removeAllEvents();

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
}

void Game::DoLoadGame()
{
	CEGUI::WindowManager::getSingleton().destroyAllWindows();
	m_ClientSocket.removeAllEvents();

	CEGUI::System::getSingleton().executeScriptFile("ScreenGame.lua");

	m_BgImage.LoadFromFile("Gui/ScreenGame.png");
	m_BgSprite.SetImage(m_BgImage);
}
