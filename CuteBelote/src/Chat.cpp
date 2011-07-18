#include "BeloteApplication.h"
#include "Chat.h"

Chat::Chat(QWidget *parent)
    : QWidget(parent)
{
    mChatUi.setupUi(this);

    connect(mChatUi.ChatInput,          SIGNAL(returnPressed()),
            this,                       SLOT(OnReturnPressed()));

    connect(&bApp->GetPlayerSocket(),   SIGNAL(SysMsgBroadcasted(const QString&)),
            this,                       SLOT(OnLogSysMsg(const QString&)));
    connect(&bApp->GetPlayerSocket(),   SIGNAL(PlayerConnectionStatusChanged(const QString&, bool)),
            this,                       SLOT(OnPlayerConnectionStatusChanged(const QString&, bool)));
    connect(&bApp->GetPlayerSocket(),   SIGNAL(TextBroadcasted(const QString&, const QString&)),
            this,                       SLOT(OnTextBroadcasted(const QString&, const QString&)));
}

void Chat::ClearLog()
{
    mChatUi.ChatContent->clear();
}

void Chat::OnLogSysMsg(const QString &msg)
{
    mChatUi.ChatContent->append(msg);
}

void Chat::OnPlayerConnectionStatusChanged(const QString &playerName, bool connected)
{
    if (connected)
        mChatUi.ChatContent->append(QString("<b>%1 a rejoint la partie</b>").arg(playerName));
    else
        mChatUi.ChatContent->append(QString("<b>%1 a quitté la partie</b>").arg(playerName));
}

void Chat::OnTextBroadcasted(const QString &sayer, const QString &msg)
{
    mChatUi.ChatContent->append(QString("<b>%1</b> : %2").arg(sayer).arg(msg));
}

void Chat::OnReturnPressed()
{
    bApp->GetPlayerSocket().SendChatMessage(mChatUi.ChatInput->text().toUtf8().constData());
    mChatUi.ChatInput->clear();
}
