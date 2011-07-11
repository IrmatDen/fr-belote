#include "GameSettings.h"

GameSettings::GameSettings(QWidget *parent)
    : QDialog(parent)
{
    mUi.setupUi(this);
}

void GameSettings::accept()
{
    QDialog::accept();
}

QString GameSettings::playerName() const
{
    return mUi.PlayerName->text();
}

PlayDirection GameSettings::playDirection() const
{
    if (mUi.PlayDirection->currentIndex() == 0)
        return PD_CW;

    return PD_CCW;
}
    
sf::Uint32 GameSettings::winningScore() const
{
    return static_cast<sf::Uint32>(mUi.MaxScore->value());
}
