#pragma once

// Screen dimensions
#define SCREEN_WIDTH 512
#define SCREEN_HEIGHT 416

//Globals
SDL_Window* g_window = nullptr;

// Function prototypes
bool InitSDL();
void CloseSDL();
bool Update();