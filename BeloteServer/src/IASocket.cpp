#include <iostream>

#include "IASocket.h"
#include "BeloteContext.h"

const std::string IASocket::BotNames[] = { "Escartefigue", "Panisse", "César", "M. Brun" };
int IASocket::k_CurrentBotIdx = 0;

IASocket::IASocket()
	: m_Seated(false)
{
	Connect("127.0.0.1", BotNames[k_CurrentBotIdx]);
	k_CurrentBotIdx++;
}

void IASocket::OnConnectionStatusChanged(ConnectionStatus newStatus)
{
	if (newStatus == CS_Connected)
	{
		SendChatMessage("Salut");
	}
}

void IASocket::OnPositionningReceived(const PositionningPacket &positionning)
{
	if (m_Seated)
		return;

	// Fill the first free spot
	boost::array<std::string, 4>::const_iterator it = std::find(positionning.m_Names.begin(),
		positionning.m_Names.end(),
		std::string());

	if (it == positionning.m_Names.end())
	{
		std::cout << "[IASocket::OnPositionningReceived] No more free seats!" << std::endl;
		Disconnect();
	}

	size_t freeSeatIdx = std::distance(positionning.m_Names.begin(), it);
	ChoosePosition(BeloteContext::PlayerPositionStrings[freeSeatIdx]);
	m_Seated = true;
}
