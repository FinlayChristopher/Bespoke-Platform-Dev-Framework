#include "Character.h"
#include "Texture2D.h"

Character::Character(SDL_Renderer* renderer, std::string imagePath, Vector2D startPosition) : GameObject(renderer, imagePath, startPosition) 
{
    //empty
}

Character::~Character() 
{
    //empty
}

void Character::Render()
{
    m_texture->Render(m_position, SDL_FLIP_NONE);
}

void Character::Update(float deltaTime, SDL_Event e)
{
    switch (e.type)
    {
    case SDL_KEYDOWN:
        switch (e.key.keysym.sym)
        {
        case SDLK_LEFT:  m_position.x -= 10; break;
        case SDLK_RIGHT: m_position.x += 10; break;
        case SDLK_a:     m_position.x -= 10; break;  // A key moves left
        case SDLK_d:     m_position.x += 10; break;  // D key moves right
        case SDLK_w:     m_position.y -= 10; break;  // W key moves up
        case SDLK_s:     m_position.y += 10; break;  // S key moves down
        }
        break;
    }
}