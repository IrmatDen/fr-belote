#ifndef BELOTE_GAME_H
#define BELOTE_GAME_H

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network/TcpSocket.hpp>

#include "Server.h"
#include "BeloteContextTypes.h"

#include "GUIManager.h"
#include "PlayerSocket.h"
#include "Singleton.h"

class Game : public Singleton<Game>
{
public:
	enum GameMode
	{
		GM_HOST,
		GM_CLIENT
	};

	enum MenuReason
	{
		MR_Normal,
		MR_ConnectionLost,
		MR_Unknown
	};

	struct GameVars
	{
		GameMode				m_GameMode;
		CEGUI::String			m_PlayerName;
		BeloteContextRuleSet	m_RuleSet;
	} m_GameVars;

public:
	static void		Init();
	void			Run();
	void			Quit();

	// UI related
	GUIManager&		GetGUIManager()								{ return m_GuiManager; }
	void			LoadMenu(MenuReason reason = MR_Normal)		{ m_MenuReason = reason; m_LoadMenuPending = true; }
	void			LoadGame()									{ m_LoadGamePending = true; }

	// Network related
	PlayerSocket&	GetPlayerSocket()			{ return m_PlayerSocket; }
	void			StartServer()				{ m_Server.SetRuleSet(m_GameVars.m_RuleSet); m_Server.Start(); }
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
	MenuReason			m_MenuReason;
	bool				m_LoadMenuPending;
	bool				m_LoadGamePending;

	// Network client vars
	PlayerSocket		m_PlayerSocket;

	// Game hosting vars
	Server				m_Server;
	
	sf::Sprite			m_BgSprite;
	sf::Image			m_BgImage;
};

#endif
