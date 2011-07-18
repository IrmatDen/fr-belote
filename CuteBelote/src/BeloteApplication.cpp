#include <QtCore/QTimer>

#include "BeloteApplication.h"

#include "TimerPool.h"

BeloteApplication::BeloteApplication(int &argc, char **argv)
    : QApplication(argc, argv), m_Server(new Server())
{
    connect(this, SIGNAL(aboutToQuit()), this, SLOT(OnAboutToQuit()));

	TimerPool::Init();

	sf::Randomizer::SetSeed((unsigned int)time(0));

    m_Server->Init();

    QTimer *updateTimer(new QTimer(this));
    updateTimer->setSingleShot(false);
    updateTimer->start(35);
    connect(updateTimer, SIGNAL(timeout()), this, SLOT(OnUpdate()));
}

BeloteApplication::~BeloteApplication()
{
}

void BeloteApplication::OnUpdate()
{
	TimerPool::getSingleton().Update();
	m_PlayerSocket.Update();
	m_Server->Update();
}

void BeloteApplication::OnAboutToQuit()
{
    StopServer();
}
