#include "SDL.h"
#include "SDL_image.h"
#include "constants.h"
#include "Texture2D.h"
#include "Commons.h"
#include "GameScreenManager.h"
#include "SDL_ttf.h"
#include <iostream>

//Globals:
SDL_Window* g_window = nullptr;
SDL_Renderer* g_renderer = nullptr;
Texture2D* g_texture = nullptr;
GameScreenManager* game_screen_manager;
Uint32 g_old_time;

int main(int argc, char* argv[])
{
	//if (SDL_Init(SDL_INIT_VIDEO) < 0)
	if (!InitSDL())
	{
		std::cout << "SDL did not initialise. Error: " << SDL_GetError();
		return false;
	}
	else
	{
		
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
	g_old_time = SDL_GetTicks();
	
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_EVENTS) < 0)
	{
		std::cout << "SDL could not initialise. SDL error: " << SDL_GetError() << std::endl;
		return false;
	}
	
	g_window = SDL_CreateWindow("Bespoke Platform Development",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		SDL_WINDOW_SHOWN);
	

	if (g_window == nullptr)
	{
		std::cout << "Window was not created. Error: " << SDL_GetError();
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

	if (TTF_Init() == -1)
	{
		std::cout << "SDL_ttf could not initialise: " << TTF_GetError() << std::endl;
		return false;
	}

	game_screen_manager = new GameScreenManager(g_renderer, SCREEN_LEVEL1);
	g_old_time = SDL_GetTicks();

	g_texture = new Texture2D(g_renderer);
	if (!g_texture->LoadFromFile("Images/TallDarkAndHandsome.PNG"))
	{
		return false;
	}
	g_texture->setSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	return true;
}

void CloseSDL()
{
	delete game_screen_manager;
	game_screen_manager = nullptr;
	delete g_texture;
	g_texture = nullptr;
	SDL_DestroyRenderer(g_renderer);
	g_renderer = nullptr;
	SDL_DestroyWindow(g_window);
	g_window = nullptr;
	TTF_Quit();
	SDL_Quit();
};

bool Update()
{
	SDL_Event e;
	SDL_PollEvent(&e);

	Uint32 new_time = SDL_GetTicks();

	game_screen_manager->Update((float)(new_time - g_old_time) / 1000.0f, e);

	g_old_time = new_time;  // update for next frame

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

	//g_texture->m_width = 800;
	//g_texture->m_height = 800;

	//g_texture->Render(Vector2D(), SDL_FLIP_NONE);
	game_screen_manager->Render();

	//Set where to render the texture
	SDL_Rect renderLocation = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	//Render the to the screen
	//SDL_RenderCopyEx(g_renderer, g_texture, NULL, &renderLocation, 0, NULL, SDL_FLIP_NONE);

	//SDL_RenderPresent(g_renderer);

	//Update the screen
	SDL_RenderPresent(g_renderer);
}

//SDL_Texture* LoadTextureFromFile(const std::string& path)
//{
//	//Remove memory used from previous texture
//	FreeTexture();
//
//	SDL_Texture* p_texture = nullptr;
//	
//	//Load the image
//	SDL_Surface* p_surface = IMG_Load(path.c_str());
//	if (p_surface != nullptr)
//	{
//		p_texture = SDL_CreateTextureFromSurface(g_renderer, p_surface);
//		if (p_texture == nullptr)
//		{
//			std::cout << "Unable to create texture from surface. Error: " << SDL_GetError() << std::endl;
//		}
//		//remove the loaded surface now we have a texture
//		SDL_FreeSurface(p_surface);
//	}
//	else
//	{
//		std::cout << "Unable to create texture from surface. Error: " << IMG_GetError() << std::endl;
//	}
//	//return the texture
//	return p_texture;
//}

//void FreeTexture()
//{
//	if (g_texture != nullptr)
//	{
//		SDL_DestroyTexture(g_texture);
//		g_texture = nullptr;
//	}
//}