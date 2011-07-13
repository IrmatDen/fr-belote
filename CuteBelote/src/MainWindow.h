#ifndef CUTE_BELOTE_MAIN_WINDOW_H
#define CUTE_BELOTE_MAIN_WINDOW_H

#include "ui_mainwindow.h"
#include "ui_chat.h"

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

private:
	Ui::MainWindow		mUi;
    Ui::Chat            mChatUi;

    GameSettings    *   mGameSettings;
    JoinGame        *   mJoinGame;
};

#endif
