#ifndef CUTE_BELOTE_GAME_SETTINGS_H
#define CUTE_BELOTE_GAME_SETTINGS_H

#include <QtGui/QDialog>

#include "../../include/BeloteContextTypes.h"

#include "ui_gamesettings.h"

class GameSettings : public QDialog
{
    Q_OBJECT

public:
    GameSettings(QWidget *parent = nullptr);

public slots:
    virtual void accept();

private:
    PlayDirection   playDirection() const;

private:
	Ui::GameSettings    mUi;
};

#endif
