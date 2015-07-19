#include <iostream>
#include <windows.h>
#include "Game.h"

using namespace std;

int main()
{
	//FreeConsole(); // Disable windows console

	Game game;
	game.Start();

	cout << "Hello, World!" << endl;
	return 0;
}