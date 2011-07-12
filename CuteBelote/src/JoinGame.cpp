#include <QtCore/QRegExp>
#include <QtGui/QRegExpValidator>
#include <QtGui/QPushButton>

#include "JoinGame.h"

JoinGame::JoinGame(QWidget *parent)
    : QDialog(parent)
{
    mUi.setupUi(this);

    QRegExp ipRegex("(?:[0-2]\\d{0,2}\\.){3}[0-2]\\d{0,2}");
    mUi.ServerIp->setValidator(new QRegExpValidator(ipRegex, this));

    mUi.buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    connect(mUi.ServerIp, SIGNAL(textChanged(const QString&)),
            this,         SLOT(onIpChanged(const QString&)));
}

QString JoinGame::playerName() const
{
    return mUi.PlayerName->text();
}
    
QString JoinGame::serverIp() const
{
    return mUi.ServerIp->text();
}

void JoinGame::accept()
{
    QDialog::accept();
}

void JoinGame::onIpChanged(const QString&)
{
    mUi.buttonBox->button(QDialogButtonBox::Ok)->setEnabled(mUi.ServerIp->hasAcceptableInput());
}
