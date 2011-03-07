#ifndef BELOTE_GAME_H
#define BELOTE_GAME_H

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network/TcpSocket.hpp>

#include "Server.h"

#include "GUIManager.h"
#include "ClientSocket.h"
#include "Singleton.h"

class Game : public Singleton<Game>
{
public:
	enum GameMode
	{
		GM_HOST,
		GM_CLIENT
	};

	struct GameVars
	{
		GameMode		m_GameMode;
		CEGUI::String	m_PlayerName;
		std::string		m_HostIP;		//!< Only used if the player tries to join a distant game.
	} m_GameVars;

public:
	static void		Init();
	void			Run();
	void			Quit();

	// UI related
	void			LoadMenu();
	void			LoadGame();

	// Network related
	void			StartServer()							{ m_ServerThread.Launch(); }
	void			StopServer()							{ m_Server.Stop(); }
	void			JoinGame(const std::string &hostIP);
	void			LeaveGame()								{ m_ClientSocket.Disconnect(); }

private:
	Game();
	~Game();
	
	sf::RenderWindow *	m_RenderWindow;
	GUIManager			m_GuiManager;

	bool				m_QuitPending;

	// Network client vars
	ClientSocket		m_ClientSocket;

	// Game hosting vars
	sf::Thread			m_ServerThread;
	Server				m_Server;
	
	sf::Sprite bgSprite;
	sf::Image bgImage;
};

#endif
