#ifndef CUTE_BELOTE_BELOTE_APPLICATION_H
#define CUTE_BELOTE_BELOTE_APPLICATION_H

#include <QtGui/QApplication>

#include "Server.h"
#include "BeloteContextTypes.h"

#include "PlayerSocket.h"

class BeloteApplication : public QApplication
{
    Q_OBJECT

public:
	enum GameMode
	{
		GM_HOST,
		GM_CLIENT
	};

	struct GameVars
	{
		GameMode				m_GameMode;
		QString			        m_PlayerName;
		BeloteContextRuleSet	m_RuleSet;
	} m_GameVars;

public:
    BeloteApplication(int &argc, char **argv);
    ~BeloteApplication();

	// Network related
	PlayerSocket&	GetPlayerSocket()		{ return m_PlayerSocket; }
	void			StartServer()			{ m_Server->SetRuleSet(m_GameVars.m_RuleSet); m_Server->Start(); }
	void			StopServer()			{ m_Server->Stop(); }

private slots:
    void OnUpdate();
    void OnAboutToQuit();

private:
	// Network client vars
	PlayerSocket		m_PlayerSocket;

	// Game hosting vars
	ServerPtr			m_Server;
};

#define bApp (static_cast<BeloteApplication *>(QCoreApplication::instance()))

#endif
