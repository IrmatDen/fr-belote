#ifndef CUTE_BELOTE_MAIN_WINDOW_H
#define CUTE_BELOTE_MAIN_WINDOW_H

#include "ui_mainwindow.h"
#include "ui_scores.h"

#include <QtCore/QSignalMapper>

#include <QtGui/QGraphicsView>
#include <QtGui/QGraphicsScene>
#include <QtGui/QPushButton>

#include "ClientSocket.h"

class GameSettings;
class JoinGame;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    virtual void resizeEvent(QResizeEvent *event);

private:
    void CreateScene();
    void ResetPosButtonText();

private slots:
    // UI events
	void OnCreateGame();
	void OnJoinGame();
    void OnLeaveGame();

    void OnPosButtonPressed(const QString &posID);

    // Network events
    void OnConnectionStatusChanged(ClientSocket::ConnectionStatus newStatus, ClientSocket::ConnectionStatus prevStatus);
    void OnPositionningReceived(const QStringList &playersPos);

private:
	Ui::MainWindow		mUi;
    Ui::Scores          mScoresUi;

    GameSettings    *   mGameSettings;
    JoinGame        *   mJoinGame;

    QGraphicsView   *   mGraphicsView;
    QGraphicsScene  *   mGraphicsScene;

    ClientSocket::ConnectionStatus  mConnectionStatus;

    QSignalMapper   *   mPositionMapper;
    QPushButton     *   mPositionButtons[4];
};

#endif
