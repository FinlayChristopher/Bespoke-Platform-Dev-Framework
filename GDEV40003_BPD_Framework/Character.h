#pragma once
#include "GameObject.h"

class Character : public GameObject
{
public:
    Character(SDL_Renderer* renderer, std::string imagePath, Vector2D startPosition);
    ~Character();
    virtual void Render() override;
    virtual void Update(float deltaTime, SDL_Event e) override;
    void ResolveCollision(float platformTop);
    float GetLastPositionY();
    bool IsGrounded() { return m_grounded; }
    void SetGrounded(bool grounded) { m_grounded = grounded; }
    float GetVelocityY() { return m_velocity_y; }
    void SetVelocityY(float v) { m_velocity_y = v; }

private:
    float m_velocity_y;
    bool m_grounded;
    bool m_jumping;
    bool m_wasSpacePressed;
    float m_last_position_y;
};