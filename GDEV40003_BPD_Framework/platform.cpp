#include "platform.h"

Platform::Platform(SDL_Renderer* renderer, std::string imagePath, Vector2D startPosition, int width, int height) : GameObject(renderer, imagePath, startPosition)
{
    m_width = width;
    m_height = height;
    m_texture->setSize(width, height);
}

Platform::~Platform() 
{
    //empty
}

void Platform::Render()
{
    m_texture->Render(m_position, SDL_FLIP_NONE);
}

void Platform::Update(float deltaTime, SDL_Event e) {}