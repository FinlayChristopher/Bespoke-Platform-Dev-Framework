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
    : GameScreen(renderer), m_screen_manager(manager)
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
    // FIRST: Check platform collisions to set m_grounded correctly
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

            float landingTolerance = platBox.h / 2.0f;

            if (lastCharBottom <= platTop + landingTolerance && my_character->GetVelocityY() >= 0)
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

    // SECOND: Now update the character with correct m_grounded state
    if (my_character)
        my_character->Update(deltaTime, e);

    // THIRD: Screen border collision detection
    Rect2D charBox = my_character->GetCollisionBox();
    
    // Left border collision
    if (charBox.x < 0)
    {
        my_character->SetPosition(Vector2D(0, my_character->GetPosition().y));
    }
    
    // Right border collision
    if (charBox.x + charBox.w > SCREEN_WIDTH)
    {
        my_character->SetPosition(Vector2D(SCREEN_WIDTH - charBox.w, my_character->GetPosition().y));
    }
    
    // Top border collision
    if (charBox.y < 0)
    {
        my_character->SetPosition(Vector2D(my_character->GetPosition().x, 0));
    }
    
    // Bottom border collision (falling off screen) - teleport to random platform
    if (charBox.y + charBox.h > SCREEN_HEIGHT)
    {
        if (!m_platforms.empty())
        {
            // Select a random platform
            int randomIndex = rand() % m_platforms.size();
            Platform* randomPlatform = m_platforms[randomIndex];
            Rect2D platBox = randomPlatform->GetCollisionBox();
            
            // Place character well above the platform to avoid getting stuck inside it
            float newX = platBox.x + (platBox.w / 2.0f) - (charBox.w / 2.0f);
            float newY = platBox.y - charBox.h - 10.0f;  // Extra 10 pixels above platform
            
            my_character->SetPosition(Vector2D(newX, newY));
            my_character->SetVelocityY(-100.0f);  // Give slight upward velocity to ensure grounded detection
            my_character->SetGrounded(false);  // Let collision system set grounded properly
        }
        else
        {
            // Fallback if no platforms exist
            my_character->SetPosition(Vector2D(64, 100));
            my_character->SetVelocityY(0);
            my_character->SetGrounded(true);
        }
    }

    // FOURTH: Check pickup collisions
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
            {
                std::cout << "All pickups collected! You've delivered all the post for the day!" << std::endl;
                
                // Transition to completion screen
                if (m_screen_manager)
                {
                    m_screen_manager->ChangeScreen(SCREEN_COMPLETION);
                }
            }
        }
    }
}

bool GameScreenLevel1::SetUpLevel()
{
    int platformCount = 5;
    int platWidth = 128;
    int platHeight = 32;
    int minVerticalSpacing = 60;
    int minHorizontalSpacing = 40;

    std::vector<Rect2D> usedPositions;

    // Create platforms first
    for (int i = 0; i < platformCount; i++)
    {
        int randX, randY;
        bool positionValid = false;
        int maxAttempts = 50;
        int attempts = 0;

        while (!positionValid && attempts < maxAttempts)
        {
            randX = rand() % (SCREEN_WIDTH - platWidth);
            randY = 200 + rand() % (SCREEN_HEIGHT - 200 - platHeight);

            positionValid = true;

            for (const Rect2D& usedPos : usedPositions)
            {
                float horizontalDist = std::abs(randX - usedPos.x);
                float verticalDist = std::abs(randY - usedPos.y);

                if (horizontalDist < minHorizontalSpacing + platWidth || 
                    verticalDist < minVerticalSpacing)
                {
                    positionValid = false;
                    break;
                }
            }

            attempts++;
        }

        if (!positionValid)
        {
            randX = rand() % (SCREEN_WIDTH - platWidth);
            randY = 200 + rand() % (SCREEN_HEIGHT - 200 - platHeight);
        }

        usedPositions.push_back(Rect2D(randX, randY, platWidth, platHeight));

        Platform* p = new Platform
        (
            m_renderer,
            "Images/YellowPlatform.png",
            Vector2D(randX, randY)
        );
        m_platforms.push_back(p);
    }

    srand(static_cast<unsigned int>(time(nullptr)));

    my_character = new Character(m_renderer, "Images/TallDarkAndHandsome.PNG", Vector2D(64, 100));

    m_background_texture = new Texture2D(m_renderer);
    if (!m_background_texture->LoadFromFile("Images/BackgroundSky.PNG"))
    {
        std::cout << "Failed to load background texture!" << std::endl;
        return false;
    }
    m_background_texture->setSize((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);

    // Generate pickups above and closer to platforms
    int pickupCount = 5;
    int pickupSize = 32;

    for (int i = 0; i < pickupCount; i++)
    {
        // Select a random platform to place the pickup near
        int platIndex = i % m_platforms.size();
        const Rect2D& platPos = usedPositions[platIndex];

        // Place pickup above the platform, centered horizontally
        int randX = platPos.x + (platPos.w / 2) - (pickupSize / 2);
        
        // Add some randomness to X position (within platform width)
        randX += (rand() % (int)platPos.w) - (platPos.w / 2);
        randX = std::max(0, std::min(randX, SCREEN_WIDTH - pickupSize));  // Clamp to screen

        // Place pickup above the platform with small random vertical offset
        int randY = platPos.y - pickupSize - 10 - (rand() % 20);  // 10-30 pixels above platform
        randY = std::max(0, randY);  // Clamp to screen top

        Pickups* pickups = new Pickups(
            m_renderer,
            "Images/letter.png",
            Vector2D(randX, randY)
        );
        m_pickups.push_back(pickups);
    }
    m_current_pickup = 0;

    return true;
}