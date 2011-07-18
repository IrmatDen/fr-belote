#include "MainWindow.h"
#include "BeloteApplication.h"
#include "GameSettings.h"
#include "JoinGame.h"

#include <QtGui/QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    mUi.setupUi(this);
    mScoresUi.setupUi(mUi.ScoresDockWidget);

    mGameSettings = new GameSettings(this);
    mJoinGame = new JoinGame(this);

	connect(mUi.actionCreateGame,       SIGNAL(triggered()), this, SLOT(OnCreateGame()));
	connect(mUi.actionJoinGame,	        SIGNAL(triggered()), this, SLOT(OnJoinGame()));
    connect(mUi.actionLeaveGame,	    SIGNAL(triggered()), this, SLOT(OnLeaveGame()));
	connect(&bApp->GetPlayerSocket(),   SIGNAL(ConnectionStatusChanged(ClientSocket::ConnectionStatus, ClientSocket::ConnectionStatus)),
            this,                       SLOT(OnConnectionStatusChanged(ClientSocket::ConnectionStatus, ClientSocket::ConnectionStatus)));

    OnConnectionStatusChanged(ClientSocket::CS_Disconnected, ClientSocket::CS_Disconnected);
}

void MainWindow::OnCreateGame()
{
    if (mGameSettings->exec() != QDialog::Accepted)
        return;

    OnConnectionStatusChanged(ClientSocket::CS_Disconnected, ClientSocket::CS_Disconnected);

    qDebug("Creating server...");
}

void MainWindow::OnJoinGame()
{
    if (mJoinGame->exec() != QDialog::Accepted)
        return;

    OnConnectionStatusChanged(ClientSocket::CS_Disconnected, ClientSocket::CS_Disconnected);

    qDebug("Joining server...");
}

void MainWindow::OnLeaveGame()
{
    bApp->GetPlayerSocket().Disconnect();
    
    if (bApp->m_GameVars.m_GameMode == BeloteApplication::GM_HOST)
        bApp->StopServer();
}

void MainWindow::OnConnectionStatusChanged(ClientSocket::ConnectionStatus newStatus, ClientSocket::ConnectionStatus prevStatus)
{
    mUi.ChatDockWidget->setEnabled(newStatus == ClientSocket::CS_Connected);
    mUi.ScoresDockWidget->setEnabled(newStatus == ClientSocket::CS_Connected);

    mUi.actionCreateGame->setEnabled(newStatus != ClientSocket::CS_Connected);
    mUi.actionJoinGame->setEnabled(newStatus != ClientSocket::CS_Connected);
    mUi.actionLeaveGame->setEnabled(newStatus == ClientSocket::CS_Connected);

    switch(prevStatus)
    {
    case ClientSocket::CS_Connected:
        if (bApp->m_GameVars.m_GameMode != BeloteApplication::GM_HOST)
            QMessageBox::critical(this, tr("CuteBelote"), tr("La connection au serveur a été perdue."));
        break;
        
    case ClientSocket::CS_LobbyFull:
        QMessageBox::critical(this, tr("CuteBelote"), tr("Il n'y a pas de place libre dans la partie."));
        break;

    case ClientSocket::CS_Disconnected:
        mUi.ChatWidgetContent->ClearLog();
        break;

    default:
        break;
    }
}