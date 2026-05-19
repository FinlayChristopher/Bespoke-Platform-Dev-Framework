#include "GameScreenCompletion.h"
#include "Texture2D.h"
#include "Constants.h"
#include <iostream>
#include <SDL.h>

GameScreenCompletion::GameScreenCompletion(SDL_Renderer* renderer) : GameScreen(renderer)
{
    SetUpScreen();
}

GameScreenCompletion::~GameScreenCompletion()
{
    delete m_background_texture;
    m_background_texture = nullptr;
}

void GameScreenCompletion::Render()
{
    if (m_background_texture)
    {
        m_background_texture->Render(Vector2D(0, 0), SDL_FLIP_NONE);
    }

    SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(m_renderer, 0x00, 0x00, 0x00, 180);
    SDL_Rect overlay = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    SDL_RenderFillRect(m_renderer, &overlay);

    SDL_SetRenderDrawColor(m_renderer, 0xFF, 0xFF, 0xFF, 255);
    SDL_Rect messageBox = { 150, 200, SCREEN_WIDTH - 300, 150 };
    SDL_RenderDrawRect(m_renderer, &messageBox);
    
    SDL_SetRenderDrawColor(m_renderer, 0x20, 0x20, 0x20, 255);
    SDL_RenderFillRect(m_renderer, &messageBox);

    SDL_SetRenderDrawColor(m_renderer, 0xFF, 0xFF, 0xFF, 255);
    SDL_RenderDrawRect(m_renderer, &messageBox);
}

void GameScreenCompletion::Update(float deltaTime, SDL_Event e)
{
    const Uint8* keystates = SDL_GetKeyboardState(NULL);

    if (keystates[SDL_SCANCODE_SPACE] || keystates[SDL_SCANCODE_RETURN])
    {
        return;
    }
}

bool GameScreenCompletion::SetUpScreen()
{
    m_background_texture = new Texture2D(m_renderer);
    if (!m_background_texture->LoadFromFile("Images/BackgroundSky.PNG"))
    {
        std::cout << "Failed to load completion screen background" << std::endl;
        return false;
    }
    m_background_texture->setSize((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);

    return true;
}