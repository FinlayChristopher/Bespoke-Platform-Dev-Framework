#include "GameScreenLevel1.h"
#include "platform.h"
#include "Texture2D.h"
#include "Constants.h"
#include "Collisions.h"
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <ctime>


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
    for (Platform* p : m_platforms)
    {
        delete p;
    }
    m_platforms.clear();
}

void GameScreenLevel1::Render()
{
    if (m_background_texture)
        m_background_texture->Render(Vector2D(0, 0), SDL_FLIP_NONE);
    if (my_character)
        my_character->Render();
    for (Platform* p : m_platforms)
    {
        p->Render();
    }
}

void GameScreenLevel1::Update(float deltaTime, SDL_Event e)
{
    // Reset grounded before Update so jump check uses last frame's value
    // then floor/platform sets it back at end of this frame
    //my_character->SetGrounded(false);

    if (my_character)
        my_character->Update(deltaTime, e);

    for (Platform* p : m_platforms)
    {
        Rect2D charBox = my_character->GetCollisionBox();
        Rect2D platBox = p->GetCollisionBox();

        if (Collisions::Instance()->Box(my_character, p))
        {
            float charBottom = charBox.y + charBox.h;
            float charTop = charBox.y;
            float charRight = charBox.x + charBox.w;
            float charLeft = charBox.x;
            float platTop = platBox.y;
            float platBottom = platBox.y + platBox.h;
            float platLeft = platBox.x;
            float platRight = platBox.x + platBox.w;
            float lastCharBottom = my_character->GetLastPositionY() + charBox.h;

            if (lastCharBottom <= platTop + 5.0f && my_character->GetVelocityY() >= 0)
            {
                my_character->ResolveCollision(platTop);
            }
            else if (charTop < platBottom && lastCharBottom > platBottom)
            {
                my_character->SetPosition(Vector2D(charBox.x, platBottom));
                my_character->SetVelocityY(0);
            }
            else if (charRight > platLeft && charLeft < platLeft)
            {
                my_character->SetPosition(Vector2D(platLeft - charBox.w, charBox.y));
            }
            else if (charLeft < platRight && charRight > platRight)
            {
                my_character->SetPosition(Vector2D(platRight, charBox.y));
            }
        }
    }
}

bool GameScreenLevel1::SetUpLevel()
{
    srand(static_cast<unsigned int>(time(nullptr)));

    my_character = new Character(m_renderer, "Images/TallDarkAndHandsome.PNG", Vector2D(64, 100));

    m_background_texture = new Texture2D(m_renderer);
    if (!m_background_texture->LoadFromFile("Images/BackgroundSky.PNG"))
    {
        std::cout << "Failed to load background texture!" << std::endl;
        return false;
    }
    m_background_texture->setSize((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);

    // Create 5 platforms in random locations
    int platformCount = 5;
    int platWidth = 128;
    int platHeight = 32;

    for (int i = 0; i < platformCount; i++)
    {
        int randX = rand() % (SCREEN_WIDTH - platWidth);
        int randY = 200 + rand() % (SCREEN_HEIGHT - 200 - platHeight);

        Platform* p = new Platform(
            m_renderer,
            "Images/YellowPlatform.PNG",
            Vector2D(randX, randY)
        );
        m_platforms.push_back(p);
    }

    return true;
}