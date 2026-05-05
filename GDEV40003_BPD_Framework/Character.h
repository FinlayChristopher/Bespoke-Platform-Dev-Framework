#pragma once
#include "GameObject.h"

class Character : public GameObject 
{
public:
    Character(SDL_Renderer* renderer, std::string imagePath, Vector2D startPosition);
    ~Character();

    virtual void Render() override;
    virtual void Update(float deltaTime, SDL_Event e) override;
};