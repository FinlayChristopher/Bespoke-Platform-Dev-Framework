#pragma once
#include "GameObject.h"

class Pickups : public GameObject
{
public:
    Pickups(SDL_Renderer* renderer, std::string imagePath, Vector2D startPosition);
    ~Pickups();
    void Render() override;
    void Update(float deltaTime, SDL_Event e) override;

    bool IsCollected() { return m_collected; }
    void Collect() { m_collected = true; }

private:
    bool m_collected;
};