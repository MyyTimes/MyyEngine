#include <iostream>
#include <SDL2/SDL.h>
#include "../Engine/Game/Game.h"

using namespace std;

const string WINDOW_NAME = "MyyEngine";

int main(int argc, char *argv[])
{
	Game game;

	if (!game.Init(WINDOW_NAME, 1280, 720))
		return -1;

	game.Run();
	return 0;
}