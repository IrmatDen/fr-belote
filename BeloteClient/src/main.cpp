#include "Game.h"

int main(int argc, char **argv)
{
	Game::Init();

	Game &g = Game::getSingleton();
	g.Run();

	return 0;
}
