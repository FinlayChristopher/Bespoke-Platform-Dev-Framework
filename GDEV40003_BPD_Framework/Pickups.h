#pragma once
#include "GameObject.h"

class Pickups : public GameObject
{
private:
    bool m_collected;

public:
    Pickups(SDL_Renderer* renderer, std::string imagePath, Vector2D startPosition);
    ~Pickups();

    void Render() override;
    void Update(float deltaTime, SDL_Event e) override;
    void Collect();
    bool IsCollected() const;
    void SetSize(float width, float height);
};