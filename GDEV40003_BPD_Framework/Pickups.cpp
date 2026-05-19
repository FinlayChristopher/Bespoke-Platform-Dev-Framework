#include "Pickups.h"
#include "Texture2D.h"

Pickups::Pickups(SDL_Renderer* renderer, std::string imagePath, Vector2D startPosition) : GameObject(renderer, imagePath, startPosition)
{
    m_collected = false;
}

Pickups::~Pickups()
{
}

void Pickups::Render()
{
    if (!m_collected)
        m_texture->Render(m_position, SDL_FLIP_NONE);
}

void Pickups::Update(float deltaTime, SDL_Event e) {}