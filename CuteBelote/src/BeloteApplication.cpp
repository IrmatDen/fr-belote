#include "BeloteApplication.h"

#include "TimerPool.h"

BeloteApplication::BeloteApplication(int &argc, char **argv)
    : QApplication(argc, argv), m_Server(new Server())
{
    connect(this, SIGNAL(aboutToQuit()), this, SLOT(OnAboutToQuit()));

	TimerPool::Init();

	sf::Randomizer::SetSeed((unsigned int)time(0));
}

BeloteApplication::~BeloteApplication()
{
}

void BeloteApplication::OnAboutToQuit()
{
    StopServer();
}
