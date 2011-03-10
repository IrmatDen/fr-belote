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
	} m_GameVars;

public:
	static void		Init();
	void			Run();
	void			Quit();

	// UI related
	void			LoadMenu()					{ m_LoadMenuPending = true; }
	void			LoadGame()					{ m_LoadGamePending = true; }

	// Network related
	ClientSocket&	GetClientSocket()			{ return m_ClientSocket; }
	void			StartServer()				{ m_ServerThread.Launch(); }
	void			StopServer()				{ m_Server.Stop(); }

private:
	Game();
	~Game();

	void	HandleEvents();
	
	void	DoLoadMenu();
	void	DoLoadGame();
	
private:
	sf::RenderWindow *	m_RenderWindow;
	GUIManager			m_GuiManager;

	// Flags
	bool				m_QuitPending;
	bool				m_LoadMenuPending;
	bool				m_LoadGamePending;

	// Network client vars
	ClientSocket		m_ClientSocket;

	// Game hosting vars
	sf::Thread			m_ServerThread;
	Server				m_Server;
	
	sf::Sprite			m_BgSprite;
	sf::Image			m_BgImage;
};

#endif
