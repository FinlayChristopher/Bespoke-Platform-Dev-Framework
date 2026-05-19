#include "Pickups.h"
#include "Texture2D.h"

Pickups::Pickups(SDL_Renderer* renderer, std::string imagePath, Vector2D startPosition)
    : GameObject(renderer, imagePath, startPosition), m_collected(false)
{
    if (m_texture)
    {
        m_texture->setSize(32.0f, 32.0f);
    }
}

Pickups::~Pickups()
{
}

void Pickups::Render()
{
    if (!m_collected && m_texture)
    {
        m_texture->Render(m_position, SDL_FLIP_NONE);
    }
}

void Pickups::Update(float deltaTime, SDL_Event e)
{
}

void Pickups::Collect()
{
    m_collected = true;
}

bool Pickups::IsCollected() const
{
    return m_collected;
}

void Pickups::SetSize(float width, float height)
{
    if (m_texture)
    {
        m_texture->setSize(width, height);
    }
}