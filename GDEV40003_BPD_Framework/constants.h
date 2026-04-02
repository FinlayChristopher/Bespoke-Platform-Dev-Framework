#pragma once
#include <SDL.h>
#include <string>

// Screen dimensions
#define SCREEN_WIDTH 512
#define SCREEN_HEIGHT 416

// Function prototypes
bool InitSDL();
void CloseSDL();
bool Update();
void Render();
SDL_Texture* LoadTextureFromFile(const std::string& path);
void FreeTexture();
