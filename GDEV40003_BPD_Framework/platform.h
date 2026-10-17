#pragma once
#include "GameObject.h"

class Platform : public GameObject
{
public:
    Platform(SDL_Renderer* renderer, std::string imagePath, Vector2D startPosition);
    ~Platform();
    void Render() override;
    void Update(float deltaTime, SDL_Event e) override;
};