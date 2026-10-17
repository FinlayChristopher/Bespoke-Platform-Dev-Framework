#pragma once
#include "GameScreen.h"
#include "Commons.h"

class Texture2D;

class GameScreenCompletion : public GameScreen
{
private:
    Texture2D* m_background_texture;
    bool SetUpScreen();
    SDL_Texture* m_text_texture = nullptr;
    SDL_Rect m_text_rect;
public:
    GameScreenCompletion(SDL_Renderer* renderer);
    ~GameScreenCompletion();

    void Render() override;
    void Update(float deltaTime, SDL_Event e) override;
};