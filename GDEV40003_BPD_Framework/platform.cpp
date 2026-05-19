#include "platform.h"
#include "Texture2D.h"

Platform::Platform(SDL_Renderer* renderer, std::string imagePath, Vector2D startPosition)
    : GameObject(renderer, imagePath, startPosition)
{
    // New dimensions: 1/2 width and 1/3 height of original
    // Original was 32x32, so: 16 width, ~10-11 height
    // Adjusting to 64x10 for better proportions
    if (m_texture)
    {
        m_texture->setSize(64.0f, 10.0f);
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