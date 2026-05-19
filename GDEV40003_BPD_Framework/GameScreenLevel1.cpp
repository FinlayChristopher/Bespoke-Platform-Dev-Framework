#include "GameScreenLevel1.h"
#include "platform.h"
#include "Texture2D.h"
#include "Constants.h"
#include "Collisions.h"
#include "GameScreenManager.h"
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <ctime>

GameScreenLevel1::GameScreenLevel1(SDL_Renderer* renderer, GameScreenManager* manager) 
    : GameScreen(renderer), m_screen_manager(manager), m_current_pickup(0)
{
    SetUpLevel();
}

GameScreenLevel1::~GameScreenLevel1()
{
    delete m_background_texture;
    m_background_texture = nullptr;
    
    delete m_character;
    m_character = nullptr;
    
    for (Platform* platform : m_platforms)
    {
        delete platform;
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
    {
        m_background_texture->Render(Vector2D(0, 0), SDL_FLIP_NONE);
    }
    
    if (m_character)
    {
        m_character->Render();
    }
    
    for (Platform* platform : m_platforms)
    {
        platform->Render();
    }
    
    if (m_current_pickup < m_pickups.size())
    {
        m_pickups[m_current_pickup]->Render();
    }
}

void GameScreenLevel1::Update(float deltaTime, SDL_Event e)
{
    CheckPlatformCollisions();
    
    if (m_character)
    {
        m_character->Update(deltaTime, e);
    }

    CheckScreenBorders();
    CheckPickupCollisions();
}

void GameScreenLevel1::CheckPlatformCollisions()
{
    for (Platform* platform : m_platforms)
    {
        Rect2D charBox = m_character->GetCollisionBox();
        Rect2D platBox = platform->GetCollisionBox();

        if (!Collisions::Instance()->Box(m_character, platform))
        {
            continue;
        }

        float charBottom = charBox.y + charBox.h;
        float charTop = charBox.y;
        float charRight = charBox.x + charBox.w;
        float charLeft = charBox.x;
        float platTop = platBox.y;
        float platBottom = platBox.y + platBox.h;
        float platLeft = platBox.x;
        float platRight = platBox.x + platBox.w;
        float lastCharBottom = m_character->GetLastPositionY() + charBox.h;

        float landingTolerance = platBox.h / 2.0f;

        if (lastCharBottom <= platTop + landingTolerance && m_character->GetVelocityY() >= 0)
        {
            m_character->ResolveCollision(platTop);
        }
        else if (charTop < platBottom && lastCharBottom > platBottom)
        {
            m_character->SetPosition(Vector2D(charBox.x, platBottom));
            m_character->SetVelocityY(0);
        }
        else if (charRight > platLeft && charLeft < platLeft)
        {
            m_character->SetPosition(Vector2D(platLeft - charBox.w, charBox.y));
        }
        else if (charLeft < platRight && charRight > platRight)
        {
            m_character->SetPosition(Vector2D(platRight, charBox.y));
        }
    }
}

void GameScreenLevel1::CheckScreenBorders()
{
    Rect2D charBox = m_character->GetCollisionBox();
    
    if (charBox.x < 0)
    {
        m_character->SetPosition(Vector2D(0, m_character->GetPosition().y));
    }
    
    if (charBox.x + charBox.w > SCREEN_WIDTH)
    {
        m_character->SetPosition(Vector2D(SCREEN_WIDTH - charBox.w, m_character->GetPosition().y));
    }
    
    if (charBox.y < 0)
    {
        m_character->SetPosition(Vector2D(m_character->GetPosition().x, 0));
    }
    
    if (charBox.y + charBox.h > SCREEN_HEIGHT)
    {
        if (!m_platforms.empty())
        {
            int randomIndex = rand() % m_platforms.size();
            Platform* randomPlatform = m_platforms[randomIndex];
            Rect2D platBox = randomPlatform->GetCollisionBox();
            
            float newX = platBox.x + (platBox.w / 2.0f) - (charBox.w / 2.0f);
            float newY = platBox.y - charBox.h - 10.0f;
            
            m_character->SetPosition(Vector2D(newX, newY));
            m_character->SetVelocityY(-100.0f);
            m_character->SetGrounded(false);
        }
        else
        {
            m_character->SetPosition(Vector2D(64, 100));
            m_character->SetVelocityY(0);
            m_character->SetGrounded(true);
        }
    }
}

void GameScreenLevel1::CheckPickupCollisions()
{
    if (m_current_pickup >= m_pickups.size())
    {
        return;
    }

    Pickups* active = m_pickups[m_current_pickup];

    if (Collisions::Instance()->Box(m_character, active))
    {
        active->Collect();
        m_current_pickup++;
        std::cout << "Pickup collected: " << m_current_pickup << " / " << m_pickups.size() << std::endl;

        if (m_current_pickup >= m_pickups.size())
        {
            std::cout << "Level complete! All pickups collected." << std::endl;
            
            if (m_screen_manager)
            {
                m_screen_manager->ChangeScreen(SCREEN_COMPLETION);
            }
        }
    }
}

bool GameScreenLevel1::SetUpLevel()
{
    srand(static_cast<unsigned int>(time(nullptr)));

    m_character = new Character(m_renderer, "Images/TallDarkAndHandsome.PNG", Vector2D(64, 100));

    m_background_texture = new Texture2D(m_renderer);
    if (!m_background_texture->LoadFromFile("Images/BackgroundSky.PNG"))
    {
        std::cout << "Failed to load background texture" << std::endl;
        return false;
    }
    m_background_texture->setSize((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);

    if (!SetUpPlatforms())
    {
        return false;
    }

    if (!SetUpPickups())
    {
        return false;
    }

    return true;
}

bool GameScreenLevel1::SetUpPlatforms()
{
    const int PLATFORM_COUNT = 5;
    const int PLATFORM_WIDTH = 128;
    const int PLATFORM_HEIGHT = 32;
    const int MIN_VERTICAL_SPACING = 60;
    const int MIN_HORIZONTAL_SPACING = 40;
    const int MAX_PLACEMENT_ATTEMPTS = 50;

    std::vector<Rect2D> usedPositions;

    for (int i = 0; i < PLATFORM_COUNT; i++)
    {
        int randX, randY;
        bool positionValid = false;
        int attempts = 0;

        while (!positionValid && attempts < MAX_PLACEMENT_ATTEMPTS)
        {
            randX = rand() % (SCREEN_WIDTH - PLATFORM_WIDTH);
            randY = 200 + rand() % (SCREEN_HEIGHT - 200 - PLATFORM_HEIGHT);

            positionValid = true;

            for (const Rect2D& usedPos : usedPositions)
            {
                float horizontalDist = std::abs(randX - usedPos.x);
                float verticalDist = std::abs(randY - usedPos.y);

                if (horizontalDist < MIN_HORIZONTAL_SPACING + PLATFORM_WIDTH || 
                    verticalDist < MIN_VERTICAL_SPACING)
                {
                    positionValid = false;
                    break;
                }
            }

            attempts++;
        }

        if (!positionValid)
        {
            randX = rand() % (SCREEN_WIDTH - PLATFORM_WIDTH);
            randY = 200 + rand() % (SCREEN_HEIGHT - 200 - PLATFORM_HEIGHT);
        }

        usedPositions.push_back(Rect2D(randX, randY, PLATFORM_WIDTH, PLATFORM_HEIGHT));

        Platform* platform = new Platform(
            m_renderer,
            "Images/YellowPlatform.png",
            Vector2D(randX, randY)
        );
        m_platforms.push_back(platform);
    }

    return true;
}

bool GameScreenLevel1::SetUpPickups()
{
    const int PICKUP_COUNT = 5;
    const int PICKUP_SIZE = 32;

    for (int i = 0; i < PICKUP_COUNT; i++)
    {
        const Rect2D& platPos = m_platforms[i % m_platforms.size()]->GetCollisionBox();

        int randX = platPos.x + (platPos.w / 2) - (PICKUP_SIZE / 2);
        randX += (rand() % (int)platPos.w) - (platPos.w / 2);
        randX = std::max(0, std::min(randX, SCREEN_WIDTH - PICKUP_SIZE));

        int randY = platPos.y - PICKUP_SIZE - 10 - (rand() % 20);
        randY = std::max(0, randY);

        Pickups* pickup = new Pickups(
            m_renderer,
            "Images/letter.png",
            Vector2D(randX, randY)
        );
        m_pickups.push_back(pickup);
    }

    m_current_pickup = 0;
    return true;
}