#include "MainWindow.h"
#include "BeloteApplication.h"
#include "GameSettings.h"
#include "JoinGame.h"

#include <QtCore/QDebug>
#include <QtGui/QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    :   QMainWindow(parent), mConnectionStatus(ClientSocket::CS_Disconnected),
        mSetupScene(nullptr), mPlayScene(nullptr), mPositionMapper(nullptr)
{
    // Setup UI
    mUi.setupUi(this);
    mScoresUi.setupUi(mUi.ScoresDockWidget);

    mGameSettings = new GameSettings(this);
    mJoinGame = new JoinGame(this);

    mGraphicsView = new QGraphicsView(this);
    setCentralWidget(mGraphicsView);

    // Init attributes
    memset(mPlayerHand, 0, sizeof(Card*) * 8);
    mPotentialAsset = nullptr;

    // Create belote scene, including widgets
    CreateSetupScene();
    CreatePlayScene();

    mGraphicsView->setScene(mPlayScene);

    // Setting up UI connections
	connect(mUi.actionCreateGame,       SIGNAL(triggered()), this, SLOT(OnCreateGame()));
	connect(mUi.actionJoinGame,	        SIGNAL(triggered()), this, SLOT(OnJoinGame()));
    connect(mUi.actionLeaveGame,	    SIGNAL(triggered()), this, SLOT(OnLeaveGame()));

    // Setting up net connections
	connect(&bApp->GetPlayerSocket(),   SIGNAL(ConnectionStatusChanged(ClientSocket::ConnectionStatus, ClientSocket::ConnectionStatus)),
            this,                       SLOT(OnConnectionStatusChanged(ClientSocket::ConnectionStatus, ClientSocket::ConnectionStatus)));
	connect(&bApp->GetPlayerSocket(),   SIGNAL(PositionningReceived(const QStringList &)),
            this,                       SLOT(OnPositionningReceived(const QStringList &)));
	connect(&bApp->GetPlayerSocket(),   SIGNAL(GameStarting()),
            this,                       SLOT(OnGameStarting()));
	connect(&bApp->GetPlayerSocket(),   SIGNAL(PlayerDealing(const QString &)),
            this,                       SLOT(OnPlayerDealing(const QString &)));
	connect(&bApp->GetPlayerSocket(),   SIGNAL(CardsDealt(const QStringList &)),
            this,                       SLOT(OnCardsDealt(const QStringList &)));
	connect(&bApp->GetPlayerSocket(),   SIGNAL(PotentialAssetReceived(const QString &)),
            this,                       SLOT(OnPotentialAssetReceived(const QString &)));
	connect(&bApp->GetPlayerSocket(),   SIGNAL(AskingRevealedAsset()),
            this,                       SLOT(OnAskingRevealedAsset()));
	connect(&bApp->GetPlayerSocket(),   SIGNAL(AcceptedAsset(int, const QString &, bool)),
            this,                       SLOT(OnAcceptedAsset(int, const QString &, bool)));

    OnConnectionStatusChanged(ClientSocket::CS_Disconnected, ClientSocket::CS_Disconnected);
}

MainWindow::~MainWindow()
{
    for (int i = 0; i != 4; i++)
    {
        delete mPositionButtons[i];
        delete mUnseatButtons[i];
    }
    delete mStartButton;
    
    delete mAcceptProposedAsset;
    delete mRefuseProposedAsset;
}

void MainWindow::CreateSetupScene()
{
    // Create setup widgets
    mPositionMapper = new QSignalMapper(this);
    QString posCodes[4] = { "South", "West", "North", "East" };
    for (int i = 0; i != 4; i++)
    {
        mPositionButtons[i] = new QPushButton;
        mPositionButtons[i]->resize(100, 25);
        mPositionMapper->setMapping(mPositionButtons[i], posCodes[i]);
        connect(mPositionButtons[i], SIGNAL(clicked()), mPositionMapper, SLOT(map()));

        mUnseatButtons[i] = new QPushButton("X");
        mUnseatButtons[i]->resize(35, 25);
        mUnseatButtons[i]->setVisible(false);

	    connect(mUnseatButtons[i], SIGNAL(clicked()), &bApp->GetPlayerSocket(), SLOT(UnseatMe()));
    }

    mStartButton = new QPushButton(tr("Jouer !"));
    mStartButton->resize(75, 25);
    connect(mStartButton, SIGNAL(clicked()), &bApp->GetPlayerSocket(), SLOT(StartGame()));

    // Build setup scene (player positionning & start button, if host)
    mSetupScene = new QGraphicsScene(this);
    mSetupScene->setBackgroundBrush(QBrush(Qt::darkGreen));
    mSetupScene->setSceneRect(0, 0, 700, 600);
    
    //QString posCodes[4]   = { "South",             "West",              "North",          "East" };
    QPointF btnPos[4]       = { QPointF(300, 567.5), QPointF(20, 287.5),  QPointF(300, 20), QPointF(580, 287.5) };
    QPointF unseatBtnPos[4] = { QPointF(405, 567.5), QPointF(125, 287.5), QPointF(405, 20), QPointF(545, 287.5) };
    for (int i = 0; i != 4; i++)
    {
        QGraphicsProxyWidget *proxy = mSetupScene->addWidget(mPositionButtons[i]);
        proxy->setPos(btnPos[i].x(), btnPos[i].y());
            
        QGraphicsProxyWidget *unseatProxy = mSetupScene->addWidget(mUnseatButtons[i]);
        unseatProxy->resize(mUnseatButtons[i]->size());
        unseatProxy->setPos(unseatBtnPos[i].x(), unseatBtnPos[i].y());
    }

    connect(mPositionMapper, SIGNAL(mapped(const QString&)), this, SLOT(OnPosButtonPressed(const QString&)));

    QGraphicsProxyWidget *startProxy = mSetupScene->addWidget(mStartButton);
    startProxy->setPos(312.5, 287.5);

    ResetPosButtonText();
}

void MainWindow::CreatePlayScene()
{
    // Build setup scene (player positionning & start button, if host)
    mPlayScene = new QGraphicsScene(this);
    mPlayScene->setBackgroundBrush(QBrush(Qt::darkGreen));
    mPlayScene->setSceneRect(0, 0, 700, 600);

    // Asset proposal widgets
    mAcceptProposedAsset = new QPushButton(tr("Accepter"));
    mAcceptProposedAsset->resize(75, 25);
    connect(mAcceptProposedAsset, SIGNAL(clicked()), this, SLOT(OnAcceptProposedAsset()));

    mAcceptWidgetProxy = mPlayScene->addWidget(mAcceptProposedAsset);
    mAcceptWidgetProxy->setPos(270, 355);
    mAcceptWidgetProxy->setVisible(false);

    mRefuseProposedAsset = new QPushButton(tr("Refuser"));
    mRefuseProposedAsset->resize(75, 25);
    connect(mRefuseProposedAsset, SIGNAL(clicked()), this, SLOT(OnRefuseProposedAsset()));

    mRefuseWidgetProxy = mPlayScene->addWidget(mRefuseProposedAsset);
    mRefuseWidgetProxy->setPos(355, 355);
    mRefuseWidgetProxy->setVisible(false);
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

    if (newStatus == ClientSocket::CS_Connected)
    {
        mGraphicsView->setScene(mSetupScene);
        mStartButton->setVisible(bApp->m_GameVars.m_GameMode == BeloteApplication::GM_HOST);
    }

    mConnectionStatus = newStatus;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);

    mGraphicsView->fitInView(0, 0, 700, 600);//, Qt::KeepAspectRatio);
}

void MainWindow::OnPosButtonPressed(const QString &posID)
{
    mMyPosition = posID;
    bApp->GetPlayerSocket().ChoosePosition(mMyPosition.toStdString());
}

void MainWindow::OnPositionningReceived(const QStringList &playersPos)
{
    Q_ASSERT(playersPos.size() == 4);

    QString playerName;
    int idx = 0;
    ResetPosButtonText();

    Q_FOREACH(playerName, playersPos)
    {
        if (playerName.size() > 0)
        {
            if (mPositionMapper->mapping(mMyPosition) == mPositionButtons[idx])
                mUnseatButtons[idx]->setVisible(true);
            else
                mUnseatButtons[idx]->setVisible(false);

            mPositionButtons[idx]->setText(mPositionButtons[idx]->text() + ": " + playerName);
            mPositionButtons[idx]->setEnabled(false);
        }
        else
        {
            mUnseatButtons[idx]->setVisible(false);
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

void MainWindow::OnGameStarting()
{
    mGraphicsView->setScene(mPlayScene);
}

void MainWindow::OnPlayerDealing(const QString &dealerName)
{
    mUi.ChatWidgetContent->OnLogSysMsg(tr("<b>%1 distribue</b>").arg(dealerName));
}

void MainWindow::OnCardsDealt(const QStringList &cardsInHand)
{
    // Clean previous hand
    for (int i = 0; i != 8; i++)
    {
        if (mPlayerHand[i]) // Just to avoid Qt's warning about removing null item
            mPlayScene->removeItem(mPlayerHand[i]);

        delete mPlayerHand[i];
    }

    // Create current hand
    int cardsCount = 0;
    QString card;
    Q_FOREACH(card, cardsInHand)
    {
        if (card.size() == 0)
            continue;

        mPlayerHand[cardsCount] = new Card(card);
        mPlayScene->addItem(mPlayerHand[cardsCount]);
        cardsCount++;
    }

    // Move each card in hand to their good position
    // FIXME-BOOOOH hardcoding cards dimensions
    const int totalWidth    = (cardsCount - 1) * 42 + 64;
    const int halfWidth     = totalWidth / 2;
    const int yPos          = 501; // 600 (play area height) - (10px from bottom) - 89px (cards' height)
    int xStartPos           = 350 - halfWidth;

    for (int cIdx = 0; cIdx != cardsCount; cIdx++, xStartPos += 42)
        mPlayerHand[cIdx]->setPos(xStartPos, yPos);
}

void MainWindow::OnPotentialAssetReceived(const QString &assetCard)
{
    if (mPotentialAsset)
    {
        mPlayScene->removeItem(mPotentialAsset);
        delete mPotentialAsset;
    }

    mPotentialAsset = new Card(assetCard);
    mPotentialAsset->setPos(318, 255);
    mPlayScene->addItem(mPotentialAsset);

    mAcceptWidgetProxy->setVisible(false);
    mRefuseWidgetProxy->setVisible(false);
}

void MainWindow::OnAskingRevealedAsset()
{
    mAcceptWidgetProxy->setVisible(true);
    mRefuseWidgetProxy->setVisible(true);
}

void MainWindow::OnAcceptProposedAsset()
{
    bApp->GetPlayerSocket().AcceptAsset(mPotentialAsset->GetCardName().toStdString());

    mAcceptWidgetProxy->setVisible(false);
    mRefuseWidgetProxy->setVisible(false);
}

void MainWindow::OnRefuseProposedAsset()
{
    bApp->GetPlayerSocket().RefuseAsset();

    mAcceptWidgetProxy->setVisible(false);
    mRefuseWidgetProxy->setVisible(false);
}

void MainWindow::OnAcceptedAsset(int takerPos, const QString &asset, bool acceptedByNSTeam)
{
    qDebug() << "Asset accepted:";
    qDebug() << "\tTaker: " << takerPos;
    qDebug() << "\tAsset: " << asset;
    qDebug() << "\tAcceptedByNSTeam: " << acceptedByNSTeam;
}
