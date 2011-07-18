#ifndef CUTE_BELOTE_CHAT_H
#define CUTE_BELOTE_CHAT_H

#include "ui_chat.h"

class Chat : public QWidget
{
	Q_OBJECT

public:
	Chat(QWidget *parent = nullptr);

    void ClearLog();

public slots:
    void OnLogSysMsg(const QString &msg);
    void OnPlayerConnectionStatusChanged(const QString &playerName, bool connected);
    void OnTextBroadcasted(const QString &sayer, const QString &msg);

private slots:
	void OnReturnPressed();

private:
    Ui::Chat    mChatUi;
};

#endif
