#include "GameScreenManager.h"
#include "GameScreen.h"
#include "GameScreenLevel1.h"
#include "Commons.h"

GameScreenManager::GameScreenManager(SDL_Renderer* renderer, SCREENS startScreen) 
{
    m_renderer = renderer;
    m_current_screen = nullptr;
    ChangeScreen(startScreen);   // immediately load the starting screen
}

GameScreenManager::~GameScreenManager() 
{
    m_renderer = nullptr;
    delete m_current_screen;
    m_current_screen = nullptr;
}

void GameScreenManager::Render() {
    m_current_screen->Render();
}

void GameScreenManager::Update(float deltaTime, SDL_Event e) 
{
    m_current_screen->Update(deltaTime, e);
}

void GameScreenManager::ChangeScreen(SCREENS new_screen) {
    if (m_current_screen != nullptr) {
        delete m_current_screen;
    }

    GameScreen* tempScreen = nullptr;

    switch (new_screen) {
    case SCREEN_LEVEL1:
        tempScreen = new GameScreenLevel1(m_renderer);
        break;
    case SCREEN_LEVEL2:
        tempScreen = new GameScreenLevel2(m_renderer);
        break;
    case SCREEN_INTRO:
        tempScreen = new GameScreenIntro(m_renderer);
        break;
    case SCREEN_MENU:
        tempScreen = new GameScreenMenu(m_renderer);
        break;
    case SCREEN_GAMEOVER:
        tempScreen = new GameScreenGameOver(m_renderer);
        break;
    case SCREEN_HIGHSCORES:
        tempScreen = new GameScreenHighScores(m_renderer);
        break;
    default:
        break;
    }

    m_current_screen = tempScreen;
}