#pragma once
#include "Commons.h"

class GameObject;

class Collisions
{
private:
    Collisions();
    static Collisions* m_instance;
public:
    ~Collisions();
    static Collisions* Instance();
    bool Circle(GameObject* a, GameObject* b);
    bool Box(GameObject* a, GameObject* b);
};

