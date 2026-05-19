#pragma once
#include "GameObject.h"

class Platform : public GameObject
{
public:
    Platform(SDL_Renderer* renderer, std::string imagePath, Vector2D startPosition, int width, int height);
    ~Platform();
    void Render() override;
    void Update(float deltaTime, SDL_Event e) override;

    Rect2D GetCollisionBox() override
    {
        return Rect2D(m_position.x, m_position.y, m_width, m_height);
    }

private:
    int m_width;
    int m_height;
};