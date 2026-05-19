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

    for (Pickups* pickup : m_pickups)
    {
        delete pickup;
    }
    m_pickups.clear();
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

    if (m_current_pickup < m_pickups.size())
    {
        m_pickups[m_current_pickup]->Render();
    }
}

void GameScreenLevel1::Update(float deltaTime, SDL_Event e)
{
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

            std::cout << "HIT! lastCharBottom: " << lastCharBottom
                << " platTop: " << platTop
                << " velocityY: " << my_character->GetVelocityY() << std::endl;

            // Top collision — was above platform last frame
            if (lastCharBottom <= platTop + 10.0f && my_character->GetVelocityY() >= 0)
            {
                my_character->ResolveCollision(platTop);
            }
            // Underside collision
            else if (my_character->GetVelocityY() < 0 && charTop < platBottom)
            {
                my_character->SetPosition(Vector2D(charBox.x, platBottom));
                my_character->SetVelocityY(0);
            }
            // Side collision — only move horizontally
            else
            {
                float overlapLeft = charRight - platLeft;
                float overlapRight = platRight - charLeft;

                if (overlapLeft < overlapRight)
                    my_character->SetPosition(Vector2D(platLeft - charBox.w, charBox.y));
                else
                    my_character->SetPosition(Vector2D(platRight, charBox.y));
            }
        }
    }

    if (m_current_pickup < m_pickups.size())
    {
        Pickups* active = m_pickups[m_current_pickup];
        if (Collisions::Instance()->Box(my_character, active))
        {
            active->Collect();
            m_current_pickup++;
            std::cout << "Pickup collected! "
                << m_current_pickup << " / "
                << m_pickups.size() << std::endl;
            if (m_current_pickup >= m_pickups.size())
                std::cout << "All pickups collected!" << std::endl;
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

    int platformCount = 5;
    int platformWidth = 128;
    int platformHeight = 24;
    int minDistanceX = platformWidth + 80;
    int minDistanceY = platformHeight + 80;

    int safetyLimit = 1000;
    int safetyCount = 0;


    for (int i = 0; i < platformCount; i++)
    {
        
        while (!validPosition && safetyCount < safetyLimit)
        {
            safetyCount++;
            randX = rand() % (SCREEN_WIDTH - platformWidth);
            randY = 150 + rand() % (SCREEN_HEIGHT - 250 - platformHeight);

            validPosition = true;

            for (Platform* existing : m_platforms)
            {
                Rect2D existingBox = existing->GetCollisionBox();
                bool tooCloseX = abs(randX - existingBox.x) < minDistanceX;
                bool tooCloseY = abs(randY - existingBox.y) < minDistanceY;

                if (tooCloseX && tooCloseY)
                {
                    validPosition = false;
                    break;
                }
            }
        }

        // Only add platform if a valid position was found
        if (safetyCount < safetyLimit)
        {
            Platform* p = new Platform(
                m_renderer,
                "Images/platform.png",
                Vector2D(randX, randY),
                platformWidth,
                platformHeight
            );
            m_platforms.push_back(p);
        }
    }
    // NO extra } here

    int pickupCount = 5;
    int attempts = 0;

    while (m_pickups.size() < pickupCount && attempts < 100)
    {
        attempts++;
        int randX = rand() % (SCREEN_WIDTH - 32);
        int randY = rand() % (SCREEN_HEIGHT - 100);

        Rect2D pickupBox(randX, randY, 32, 32);

        bool overlaps = false;
        for (Platform* p : m_platforms)
        {
            Rect2D platBox = p->GetCollisionBox();
            if (pickupBox.x < platBox.x + platBox.w &&
                pickupBox.x + pickupBox.w > platBox.x &&
                pickupBox.y < platBox.y + platBox.h &&
                pickupBox.y + pickupBox.h > platBox.y)
            {
                overlaps = true;
                break;
            }
        }

        if (!overlaps)
        {
            Pickups* pickup = new Pickups(
                m_renderer,
                "Images/letter.PNG",
                Vector2D(randX, randY)
            );
            m_pickups.push_back(pickup);
        }
    }
    m_current_pickup = 0;

    return true;
}