#include "GameScreenLevel1.h"
#include "Texture2D.h"   // keeping the promise
#include <iostream>

GameScreenLevel1::GameScreenLevel1(SDL_Renderer* renderer) : GameScreen(renderer) 
{
    SetUpLevel();
}

GameScreenLevel1::~GameScreenLevel1() 
{
    delete m_background_texture;
    m_background_texture = nullptr;

    delete my_character;
    my_character = nullptr;
}

void GameScreenLevel1::Render() 
{
    m_background_texture->Render(Vector2D(), SDL_FLIP_NONE);
    my_character->Render();
}

void GameScreenLevel1::Update(float deltaTime, SDL_Event e) 
{
    my_character->Update(deltaTime, e);
}

bool GameScreenLevel1::SetUpLevel() 
{
    //my_character = new Character(m_renderer, "Images/TallDarkAndHandsome.png", Vector2D(64, 330));

    m_background_texture = new Texture2D(m_renderer);
    if (!m_background_texture->LoadFromFile("Images/TallDarkAndHandsome.PNG")) 
    {
        std::cout << "Failed to load background texture!" << std::endl;
        return false;
    }

    return true;
}

bool GameScreenLevel1::SetUpLevel()
{
    if (m_renderer == nullptr)
    {
        std::cout << "m_renderer is null at start of SetUpLevel!" << std::endl;
        return false;
    }

    m_background_texture = new Texture2D(m_renderer);
    if (!m_background_texture->LoadFromFile("Images/TallDarkAndHandsome.PNG"))
    {
        std::cout << "Failed to load background texture!" << std::endl;
        return false;
    }
    return true;
}