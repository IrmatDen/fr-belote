#include "MainWindow.h"
#include "GameSettings.h"
#include "JoinGame.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    mUi.setupUi(this);
    mChatUi.setupUi(mUi.ChatDockWidget);
    mScoresUi.setupUi(mUi.ScoresDockWidget);

	connect(mUi.actionCreateGame,   SIGNAL(triggered()), this, SLOT(OnCreateGame()));
	connect(mUi.actionJoinGame,	    SIGNAL(triggered()), this, SLOT(OnJoinGame()));

    mGameSettings = new GameSettings(this);
    mJoinGame = new JoinGame(this);
}

void MainWindow::OnCreateGame()
{
    if (mGameSettings->exec() != QDialog::Accepted)
        return;

    qDebug("Creating server...");
}

void MainWindow::OnJoinGame()
{
    if (mJoinGame->exec() != QDialog::Accepted)
        return;

    qDebug("Joining server...");
}