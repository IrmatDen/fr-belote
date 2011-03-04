#include <iostream>

#include <CEGUI.h>

#include <SFML/Network/IpAddress.hpp>

#include "Game.h"
#include "SoundManager.h"

Game* Singleton<Game>::ms_Singleton = 0;

Game::Game()
	: m_RenderWindow(0), m_QuitPending(false), m_ServerThread(&Server::Start, &m_Server)
{
	m_RenderWindow = new sf::RenderWindow(sf::VideoMode(800, 600, 32), "Belote");
	m_RenderWindow->ShowMouseCursor(false);

	m_GuiManager.Initialize(m_RenderWindow);


	///////// TEMP
	CEGUI::System::getSingleton().executeScriptFile("MenuScreenMain.lua");

	bgImage.LoadFromFile("Gui/MenuScreenMain.png");
	bgSprite.SetImage(bgImage);
}

Game::~Game()
{
}

void Game::Init()
{
	new Game();
	SoundManager::Init();
}

void Game::Run()
{
	sf::Event event;
	while (m_RenderWindow->IsOpened())
	{
		while (!m_QuitPending && m_RenderWindow->GetEvent(event))
		{
			if (m_GuiManager.HandleEvent(event))
				continue;

			switch (event.Type)
			{
			case sf::Event::Closed:
				Quit();
				break;

			case sf::Event::KeyPressed:
				{
					switch (event.Key.Code)
					{
					case sf::Key::Escape:
						Quit();
						break;

					default:
						break;
					}
				}
				
			default:
				break;
			}
		}

		if (m_QuitPending)
		{
			// Wait until the click sound has finished playing (if the player clicked on a Quit button).
			if (!SoundManager::getSingleton().IsFXPlaying(SoundManager::FX_CLICK))
				m_RenderWindow->Close();
		}

		m_RenderWindow->Draw(bgSprite);
		m_GuiManager.Draw();
		m_RenderWindow->Display();
	}
}

void Game::Quit()
{
	m_QuitPending = true;
}

void Game::JoinGame(const std::string &hostIP)
{
	m_ClientSocket.Connect(hostIP, m_GameVars.m_PlayerName.c_str());
}
