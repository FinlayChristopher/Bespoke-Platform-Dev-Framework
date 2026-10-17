#include "Character.h"
#include "Texture2D.h"

const float GRAVITY = 980.0f;
const float JUMP_FORCE = -500.0f;
const float MOVE_SPEED = 200.0f;

Character::Character(SDL_Renderer* renderer, std::string imagePath, Vector2D startPosition)
    : GameObject(renderer, imagePath, startPosition)
{
    if (m_texture)
    {
        m_texture->setSize(32.0f, 32.0f);
    }
    
    m_velocity_y = 0.0f;
    m_grounded = false;
    m_jumping = false;
    m_wasSpacePressed = false;
    m_last_position_y = startPosition.y;
}

Character::~Character() 
{
}

void Character::Render()
{
    m_texture->Render(m_position, SDL_FLIP_NONE);
}

void Character::Update(float deltaTime, SDL_Event e)
{
    HandleInput(deltaTime);
    ApplyPhysics(deltaTime);
}

void Character::HandleInput(float deltaTime)
{
    const Uint8* keystates = SDL_GetKeyboardState(NULL);

    if (keystates[SDL_SCANCODE_LEFT] || keystates[SDL_SCANCODE_A])
    {
        m_position.x -= MOVE_SPEED * deltaTime;
    }
    
    if (keystates[SDL_SCANCODE_RIGHT] || keystates[SDL_SCANCODE_D])
    {
        m_position.x += MOVE_SPEED * deltaTime;
    }

    bool spaceHeld = keystates[SDL_SCANCODE_SPACE] || keystates[SDL_SCANCODE_W];
    
    if (spaceHeld && !m_wasSpacePressed && m_grounded)
    {
        m_velocity_y = JUMP_FORCE;
        m_grounded = false;
        m_jumping = true;
    }
    
    m_wasSpacePressed = spaceHeld;
}

void Character::ApplyPhysics(float deltaTime)
{
    m_last_position_y = m_position.y;
    
    m_velocity_y += GRAVITY * deltaTime;
    m_position.y += m_velocity_y * deltaTime;
}

void Character::ResolveCollision(float platformTop)
{
    m_position.y = platformTop - m_texture->GetHeight();
    m_velocity_y = 0.0f;
    m_grounded = true;
    m_jumping = false;
}

float Character::GetLastPositionY()
{
    return m_last_position_y;
}