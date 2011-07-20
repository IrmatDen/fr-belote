#include "MainWindow.h"
#include "BeloteApplication.h"
#include "GameSettings.h"
#include "JoinGame.h"
#include "Card.h"

#include <QtGui/QGraphicsProxyWidget>
#include <QtGui/QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    :   QMainWindow(parent), mConnectionStatus(ClientSocket::CS_Disconnected),
        mGraphicsScene(nullptr), mPositionMapper(nullptr)
{
    mUi.setupUi(this);
    mScoresUi.setupUi(mUi.ScoresDockWidget);

    mGameSettings = new GameSettings(this);
    mJoinGame = new JoinGame(this);

    mGraphicsView = new QGraphicsView(this);
    setCentralWidget(mGraphicsView);

    // Create belote scene, including widgets
    CreateScene();
    mPositionMapper = new QSignalMapper(this);

    QString posCodes[4]  = { "South",   "West",      "North",    "East" };

    for (int i = 0; i != 4; i++)
    {
        mPositionButtons[i] = new QPushButton;
        mPositionButtons[i]->resize(100, 25);
        mPositionMapper->setMapping(mPositionButtons[i], posCodes[i]);
        connect(mPositionButtons[i], SIGNAL(clicked()), mPositionMapper, SLOT(map()));
    }

    // Setting up UI connections
	connect(mUi.actionCreateGame,       SIGNAL(triggered()), this, SLOT(OnCreateGame()));
	connect(mUi.actionJoinGame,	        SIGNAL(triggered()), this, SLOT(OnJoinGame()));
    connect(mUi.actionLeaveGame,	    SIGNAL(triggered()), this, SLOT(OnLeaveGame()));
    connect(mPositionMapper,            SIGNAL(mapped(const QString&)),
            this,                       SLOT(OnPosButtonPressed(const QString&)));

    // Setting up net connections
	connect(&bApp->GetPlayerSocket(),   SIGNAL(ConnectionStatusChanged(ClientSocket::ConnectionStatus, ClientSocket::ConnectionStatus)),
            this,                       SLOT(OnConnectionStatusChanged(ClientSocket::ConnectionStatus, ClientSocket::ConnectionStatus)));
	connect(&bApp->GetPlayerSocket(),   SIGNAL(PositionningReceived(const QStringList &)),
            this,                       SLOT(OnPositionningReceived(const QStringList &)));

    OnConnectionStatusChanged(ClientSocket::CS_Disconnected, ClientSocket::CS_Disconnected);
}

MainWindow::~MainWindow()
{
    for (int i = 0; i != 4; i++)
        delete mPositionButtons[i];
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

    mConnectionStatus = newStatus;

    CreateScene();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);

    mGraphicsView->fitInView(0, 0, 700, 600);//, Qt::KeepAspectRatio);
}

void MainWindow::CreateScene()
{
    delete mGraphicsScene;
    mGraphicsScene = new QGraphicsScene(this);
    mGraphicsScene->setBackgroundBrush(QBrush(Qt::darkGreen));
    mGraphicsScene->setSceneRect(0, 0, 700, 600);

    // Setup positionning widgets if we just connected
    if (mConnectionStatus == ClientSocket::CS_Connected)
    {
        //QString posCodes[4]   = { "South",             "West",             "North",          "East" };
        QPointF btnPos[4]       = { QPointF(300, 567.5), QPointF(20, 287.5), QPointF(300, 20), QPointF(580, 287.5) };
        for (int i = 0; i != 4; i++)
        {
            QGraphicsProxyWidget *proxy = mGraphicsScene->addWidget(mPositionButtons[i]);
            proxy->setPos(btnPos[i].x(), btnPos[i].y());
        }

        ResetPosButtonText();
    }

    mGraphicsView->setScene(mGraphicsScene);
}

void MainWindow::OnPosButtonPressed(const QString &posID)
{
    bApp->GetPlayerSocket().ChoosePosition(posID.toStdString());
}

void MainWindow::OnPositionningReceived(const QStringList &playersPos)
{
    Q_ASSERT(playersPos.size() == 4);

    QString pp;
    int idx = 0;
    ResetPosButtonText();

    Q_FOREACH(pp, playersPos)
    {
        if (pp.size() > 0)
        {
            mPositionButtons[idx]->setText(mPositionButtons[idx]->text() + ": " + pp);
            mPositionButtons[idx]->setEnabled(false);
        }

        idx++;
    }
}

void MainWindow::ResetPosButtonText()
{
    QString posLabels[4] = { tr("Sud"), tr("Ouest"), tr("Nord"), tr("Est") };

    for (int i = 0; i != 4; i++)
    {
        mPositionButtons[i]->setText(posLabels[i]);
        mPositionButtons[i]->setEnabled(true);
    }
}
