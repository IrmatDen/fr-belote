#ifndef CUTE_BELOTE_MAIN_WINDOW_H
#define CUTE_BELOTE_MAIN_WINDOW_H

#include "ui_mainwindow.h"

#include <QtGui/QMainWindow>

class GameSettings;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);

private slots:
	void OnCreateGame();
	void OnJoinGame();

private:
	Ui::MainWindow			mUi;

    GameSettings        *   mGameSettings;
};

#endif
