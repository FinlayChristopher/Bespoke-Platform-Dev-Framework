#pragma once
#include "Commons.h"
#include "Texture2D.h"
#include <SDL.h>
#include <iostream>

class Texture2D;

class GameObject 
{
protected:
    SDL_Renderer* m_renderer;
    Vector2D m_position;
    Texture2D* m_texture;
    float m_collision_radius;

public:
    GameObject(SDL_Renderer* renderer, std::string imagePath, Vector2D start_position);
    ~GameObject();
    float GetCollisionRadius();

    virtual void Render();
    virtual void Update(float deltaTime, SDL_Event e);

    void SetPosition(Vector2D new_position);
    Vector2D GetPosition();

    virtual Rect2D GetCollisionBox()
    {
        return Rect2D(m_position.x, m_position.y,
            m_texture->GetWidth(), m_texture->GetHeight());
    }
};