#include "GameScreen.h"

GameScreen::GameScreen(SDL_Renderer* renderer) {
    m_renderer = renderer;  // store the renderer reference
}

GameScreen::~GameScreen() {
    m_renderer = nullptr;   // don't delete — we don't own it
}

void GameScreen::Render() {}   // empty — subclasses override

void GameScreen::Update(float deltaTime, SDL_Event e) {}