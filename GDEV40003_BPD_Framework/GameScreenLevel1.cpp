#include "GameScreenLevel1.h"
#include "Texture2D.h"
#include "Constants.h"
#include "Collisions.h"
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
   if (m_background_texture)
        m_background_texture->Render(Vector2D(0,0), SDL_FLIP_NONE);

    if (my_character)
        my_character->Render();
    //my_character->Render();
}

void GameScreenLevel1::Update(float deltaTime, SDL_Event e) 
{
    if (my_character)
        my_character->Update(deltaTime, e);
    
    if (Collisions::Instance()->Circle(my_character))
    {
        std::cout << "Circle hit!" << std::endl;
    }

    if (Collisions::Instance()->Box(my_character))
    {
        std::cout << "Box hit!" << std::endl;
    }

}

bool GameScreenLevel1::SetUpLevel()
{
    //my_character = new Character(m_renderer, "Images/TallDarkAndHandsome.PNG", Vector2D(64, 330));

    m_background_texture = new Texture2D(m_renderer);
    if (!m_background_texture->LoadFromFile("Images/TallDarkAndHandsome.PNG"))
    {
        std::cout << "Failed to load background texture!" << std::endl;
        return false;
    }

    m_background_texture->setSize(SCREEN_WIDTH, SCREEN_HEIGHT);


    return true;
}