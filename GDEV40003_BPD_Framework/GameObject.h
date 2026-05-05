#pragma once
#include <SDL.h>
#include <iostream>
#include "Commons.h"

class Texture2D;

class GameObject 
{
protected:
    SDL_Renderer* m_renderer;
    Vector2D      m_position;
    Texture2D* m_texture;

public:
    GameObject(SDL_Renderer* renderer, std::string imagePath, Vector2D start_position);
    ~GameObject();

    virtual void Render();
    virtual void Update(float deltaTime, SDL_Event e);

    void    SetPosition(Vector2D new_position);
    Vector2D GetPosition();
};