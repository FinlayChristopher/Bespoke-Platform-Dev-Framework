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
        case SDLK_LEFT:  m_position.x -= 1; break;
        case SDLK_RIGHT: m_position.x += 1; break;
        }
        break;
    }
}