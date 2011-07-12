#include "BeloteApplication.h"

BeloteApplication::BeloteApplication(int &argc, char **argv)
    : QApplication(argc, argv)
{
    connect(this, SIGNAL(aboutToQuit()), this, SLOT(OnAboutToQuit()));
}

void BeloteApplication::OnAboutToQuit()
{
    StopServer();
}
