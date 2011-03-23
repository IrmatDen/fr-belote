#include "IASocket.h"

const std::string IASocket::BotNames[] = { "Escartefigue", "Panisse", "César", "M. Brun" };
int IASocket::k_CurrentBotIdx = 0;

IASocket::IASocket()
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
