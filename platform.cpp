#include "platform.h"
#include "Texture2D.h"

Platform::Platform(SDL_Renderer* renderer, std::string imagePath, Vector2D startPosition)
    : GameObject(renderer, imagePath, startPosition)
{
    // Override the default 64x64 size with correct platform dimensions
    // Your sprite: 32 width x 9 height
    if (m_texture)
    {
        m_texture->setSize(32.0f, 9.0f);
    }
}

Platform::~Platform()
{
}

void Platform::Render()
{
    m_texture->Render(m_position, SDL_FLIP_NONE);
}

void Platform::Update(float deltaTime, SDL_Event e)
{
}