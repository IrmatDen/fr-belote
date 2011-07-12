#ifndef CUTE_BELOTE_JOIN_GAME_H
#define CUTE_BELOTE_JOIN_GAME_H

#include <QtGui/QDialog>

#include "ui_joingame.h"

class JoinGame : public QDialog
{
    Q_OBJECT

public:
    JoinGame(QWidget *parent = nullptr);

    QString         playerName() const;

    QString         serverIp() const;

public slots:
    virtual void accept();

private slots:
    void onIpChanged(const QString&);

private:
	Ui::JoinGame    mUi;
};

#endif
