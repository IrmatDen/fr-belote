#include "GameSettings.h"
#include "BeloteApplication.h"

GameSettings::GameSettings(QWidget *parent)
    : QDialog(parent)
{
    mUi.setupUi(this);
}

PlayDirection GameSettings::playDirection() const
{
    if (mUi.PlayDirection->currentIndex() == 0)
        return PD_CW;

    return PD_CCW;
}

void GameSettings::accept()
{
    // Save game settings
    bApp->m_GameVars.m_PlayerName               = mUi.PlayerName->text();
	bApp->m_GameVars.m_RuleSet.m_PlayDir        = playDirection();
	bApp->m_GameVars.m_RuleSet.m_WinningScore   = static_cast<sf::Uint32>(mUi.MaxScore->value());
    bApp->m_GameVars.m_RuleSet.m_AllowBots      = (mUi.AllowBots->checkState() == Qt::Checked);

    // Start local server
    bApp->StartServer();
    bApp->GetPlayerSocket().Connect("127.0.0.1", bApp->m_GameVars.m_PlayerName.toStdString());

    QDialog::accept();
}
