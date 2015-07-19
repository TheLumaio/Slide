#include <iostream>
#include "Game.h"

#if defined(WIN32) || defined(_WIN32)
#include <windows.h>
#endif

using namespace std;

int main()
{
	#if defined(WIN32) || defined(_WIN32)
	FreeConsole(); // Disable windows console
	#endif

	Game game;
	game.loadScore();
	game.Start();

	cout << "Hello, World!" << endl;
	return 0;
}