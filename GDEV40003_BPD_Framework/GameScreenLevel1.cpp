#include "GameScreenLevel1.h"
#include "Texture2D.h"   // keeping the promise
#include <iostream>

GameScreenLevel1::GameScreenLevel1(SDL_Renderer* renderer) : GameScreen(renderer) {
    SetUpLevel();
}

GameScreenLevel1::~GameScreenLevel1() {
    delete m_background_texture;
    m_background_texture = nullptr;
}

void GameScreenLevel1::Render() {
    m_background_texture->Render(Vector2D(), SDL_FLIP_NONE);
}

void GameScreenLevel1::Update(float deltaTime, SDL_Event e) {
    // empty for now
}

bool GameScreenLevel1::SetUpLevel() {
    m_background_texture = new Texture2D(m_renderer);
    if (!m_background_texture->LoadFromFile("Images/TallDarkAndHandsome.png")) 
    {
        std::cout << "Failed to load background texture!" << std::endl;
        return false;
    }
    return true;
}