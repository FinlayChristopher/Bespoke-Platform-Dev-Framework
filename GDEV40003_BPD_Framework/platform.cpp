#include "platform.h"

Platform::Platform(SDL_Renderer* renderer, std::string imagePath, Vector2D startPosition)
    : GameObject(renderer, imagePath, startPosition)
{
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