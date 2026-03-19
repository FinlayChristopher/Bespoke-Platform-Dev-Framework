#include "SDL.h"
#include "constants.h"
#include <iostream>

//Globals
SDL_Window* g_window = nullptr;

int main(int argc, char* argv[])
{
	//Set up SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "SDL did not initialise. Error: " << SDL_GetError();
		return false;
	}
	else
	{
		g_window = SDL_CreateWindow("Bespoke Platform Development",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			SDL_WINDOW_SHOWN);
		if (g_window == nullptr)
		{
			//Window failed
			std::cout << "Window was not created. Error: " << SDL_GetError();
			return false;
		}
	}
	return 0;
}
