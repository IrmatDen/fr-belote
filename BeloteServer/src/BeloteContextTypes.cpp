#include "BeloteContextTypes.h"

const std::string	ColourPreffixes("HSDC");

const std::string	CardDefToScore::ValueOrder("789JQK101");
const int			CardDefToScore::NormalScores[] = {0, 0, 0, 2, 3, 4, 10, 0xFFFF, 11};
const std::string	CardDefToScore::ValueOrderAtAsset("78QK1019J");
const int			CardDefToScore::AssetScores[] = {0, 0, 3, 4, 10, 0xFFFF, 11, 14, 20};
