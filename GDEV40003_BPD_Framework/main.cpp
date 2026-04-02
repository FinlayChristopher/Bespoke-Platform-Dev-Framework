#include "SDL.h"
#include "SDL_image.h"
#include "constants.h"
#include <iostream>

//Globals
SDL_Window* g_window = nullptr;
SDL_Renderer* g_renderer = nullptr;
SDL_Texture* g_texture = nullptr;

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
		Render();
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

	g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);

	if (g_renderer != nullptr)
	{
		//init png loading
		int imageFlags = IMG_INIT_PNG;
		if(!(IMG_Init(imageFlags) & imageFlags))
		{
			std::cout << "SDL_image could not initialise. SDL_image error: " << IMG_GetError() << std::endl;
			return false;
		}
	}
	else
	{
		std::cout << "Renderer could not initialise. Renderer error: " << SDL_GetError() << std::endl;
		return false;
	}

	g_texture = LoadTextureFromFile("Images\TallDarkAndHandsome.png");
	if (g_texture == nullptr)
	{
		return false;
	}
}

void CloseSDL()
{
	SDL_DestroyWindow(g_window);
	g_window = nullptr;

	SDL_Quit();
	CloseSDL();

	//Clear the texture
	FreeTexture();
	//Release the renderer
	SDL_DestroyRenderer(g_renderer);
	g_renderer = nullptr;
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

void Render()
{
	//Clear the screen
	SDL_SetRenderDrawColor(g_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(g_renderer);

	//Set where to render the texture
	SDL_Rect renderLocation = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	//Render the to the screen
	SDL_RenderCopyEx(g_renderer, g_texture, NULL, &renderLocation, 0, NULL, SDL_FLIP_NONE);

	//Update the screen
	SDL_RenderPresent(g_renderer);
}

SDL_Texture* LoadTextureFromFile(const std::string& path)
{
	//Remove memory used from previous texture
	FreeTexture();

	SDL_Texture* p_texture = nullptr;
	
	//Load the image
	SDL_Surface* p_surface = IMG_Load(path.c_str());
	if (p_surface != nullptr)
	{
		p_texture = SDL_CreateTextureFromSurface(g_renderer, p_surface);
		if (p_texture == nullptr)
		{
			std::cout << "Unable to create texture from surface. Error: " << SDL_GetError() << std::endl;
		}
		//remove the loaded surface now we have a texture
		SDL_FreeSurface(p_surface);
	}
	else
	{
		std::cout << "Unable to create texture from surface. Error: " << IMG_GetError() << std::endl;
	}
	//return the texture
	return p_texture;
}

void FreeTexture()
{
	if (g_texture != nullptr)
	{
		SDL_DestroyTexture(g_texture);
		g_texture = nullptr;
	}
}