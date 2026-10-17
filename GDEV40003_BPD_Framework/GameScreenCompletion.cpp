#include "GameScreenCompletion.h"
#include "Texture2D.h"
#include "Constants.h"
#include "SDL_ttf.h"
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
    SDL_DestroyTexture(m_text_texture);
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
    
    SDL_SetRenderDrawColor(m_renderer, 0x20, 0x20, 0x20, 255);
    SDL_RenderFillRect(m_renderer, &messageBox);

    SDL_SetRenderDrawColor(m_renderer, 0xFF, 0xFF, 0xFF, 255);
    SDL_RenderDrawRect(m_renderer, &messageBox);
    if (m_text_texture)
    {
        m_text_rect.x = messageBox.x + (messageBox.w - m_text_rect.w) / 2;
        m_text_rect.y = messageBox.y + (messageBox.h - m_text_rect.h) / 2;
        SDL_RenderCopy(m_renderer, m_text_texture, nullptr, &m_text_rect);
    }
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
    if (!m_background_texture->LoadFromFile("Images/BackgroundSky.png"))
    {
        std::cout << "Failed to load completion screen background" << std::endl;
        return false;
    }
    m_background_texture->setSize((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);

    TTF_Font* font = TTF_OpenFont("Fonts/LiberationSerif-Regular.ttf", 32);
    if (font == nullptr)
    {
        std::cout << "Failed to load font: " << TTF_GetError() << std::endl;
        return false;
    }
    
    SDL_Color white = { 255, 255, 255, 255 };
    SDL_Surface* surface = TTF_RenderText_Blended(font, "Level complete!", white);
    TTF_CloseFont(font);

    if (surface == nullptr)
    {
        std::cout << "Failed to render text: " << TTF_GetError() << std::endl;
        return false;
    }

    m_text_texture = SDL_CreateTextureFromSurface(m_renderer, surface);
    m_text_rect = { 0, 0, surface->w, surface->h };
    SDL_FreeSurface(surface);

    return true;
}