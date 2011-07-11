#include "MainWindow.h"
#include "GameSettings.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    mUi.setupUi(this);

	connect(mUi.actionCreateGame,   SIGNAL(triggered()), this, SLOT(OnCreateGame()));
	connect(mUi.actionJoinGame,	    SIGNAL(triggered()), this, SLOT(OnJoinGame()));

    mGameSettings = new GameSettings(this);
}

void MainWindow::OnCreateGame()
{
    if (mGameSettings->exec() != QDialog::Accepted)
        return;

    qDebug("Creating server...");
}

void MainWindow::OnJoinGame()
{
}