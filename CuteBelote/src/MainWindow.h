#ifndef CUTE_BELOTE_MAIN_WINDOW_H
#define CUTE_BELOTE_MAIN_WINDOW_H

#include "ui_mainwindow.h"
#include "ui_scores.h"

#include "ClientSocket.h"

class GameSettings;
class JoinGame;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);

private slots:
	void OnCreateGame();
	void OnJoinGame();
    void OnLeaveGame();
    void OnConnectionStatusChanged(ClientSocket::ConnectionStatus newStatus, ClientSocket::ConnectionStatus prevStatus);

private:
	Ui::MainWindow		mUi;
    Ui::Scores          mScoresUi;

    GameSettings    *   mGameSettings;
    JoinGame        *   mJoinGame;
};

#endif
