#ifndef GAMESCREENLEVEL1_H
#define GAMESCREENLEVEL1_H
#include "GameScreen.h"
#include "Commons.h"
#include "Character.h"
#include "platform.h"
#include "Pickups.h"
#include <vector>

class Character;
class Texture2D;
class GameScreenManager;

class GameScreenLevel1 : public GameScreen 
{
private:
    Texture2D* m_background_texture;
    Character* m_character;
    std::vector<Platform*> m_platforms;
    std::vector<Pickups*> m_pickups;
    int m_current_pickup;
    GameScreenManager* m_screen_manager;

    bool SetUpLevel();
    bool SetUpPlatforms();
    bool SetUpPickups();
    void CheckPlatformCollisions();
    void CheckScreenBorders();
    void CheckPickupCollisions();
public:
    GameScreenLevel1(SDL_Renderer* renderer, GameScreenManager* manager = nullptr);
    ~GameScreenLevel1();
    void Render() override;
    void Update(float deltaTime, SDL_Event e) override;
};
#endif
