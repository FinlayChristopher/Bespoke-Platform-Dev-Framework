#include "SDL.h"
#include "constants.h"
#include <iostream>

int main(int argc, char* argv[])
{
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

	//Flag to check if we wish to quit
	bool quit = false;

	//Game loop
	while (!quit)
	{
		quit = Update();
	}
	
	return 0;
}

bool InitSDL()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_EVENTS) < 0)
	{
		std::cout << "SDL could not initialise. SDL error: " << SDL_GetError() << std::endl;
		return false;
	}

	SDL_JoystickOpen(1);



	return 0;
}

void CloseSDL()
{
	SDL_DestroyWindow(g_window);
	g_window = nullptr;

	SDL_Quit();
	CloseSDL();
};

bool Update()
{
	SDL_Event e;
	SDL_PollEvent(&e);

	switch(e.type)
	{
		//Click 'x' to quit
		case SDL_QUIT:
			return true;
			
		case SDL_KEYDOWN:
			switch (e.key.keysym.sym)
			{
			case SDLK_a:
				std::cout << "key pressed" << std::endl;
				break;
			}
			break;
		case SDL_JOYBUTTONDOWN:
			switch (e.jbutton.button)
			{
				std::cout << "button pressed" << std::endl;
			}
		case SDL_JOYBUTTONUP:
			switch (e.jbutton.button)
			{
				std::cout << "button released" << std::endl;
				break;
			}
		default:
			break;
	}
	return false;
}