#include "BeloteContextTypes.h"

const std::string	BeloteUtils::ColourPreffixes("HSDC");
const std::string	BeloteUtils::ValueOrder("789JQK101");
const int			BeloteUtils::NormalScores[] = {0, 0, 0, 2, 3, 4, 10, 0xFFFF, 11};
const std::string	BeloteUtils::ValueOrderAtAsset("78QK1019J");
const int			BeloteUtils::AssetScores[] = {0, 0, 3, 4, 10, 0xFFFF, 11, 14, 20};

int BeloteUtils::GetCardIndex(const std::string &card, const std::string &asset)
{
	const bool isAsset			= !asset.empty() && card.front() == asset.front();
	const char * const cardVal	= card.c_str() + 1;

	int cardIdx = -1;
	if (isAsset)
	{
		cardIdx = ValueOrderAtAsset.rfind(cardVal);
		if (cardIdx > 5)
			cardIdx--;
	}
	else
	{
		cardIdx = ValueOrder.rfind(cardVal);
		if (cardIdx == 8)
			cardIdx--;
	}

	return cardIdx;
}
