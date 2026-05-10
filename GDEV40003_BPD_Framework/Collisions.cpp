#include "Collisions.h"
#include "GameObject.h"

// Define and initialise the static instance pointer
Collisions* Collisions::m_instance = nullptr;

Collisions::Collisions() 
{
}

Collisions::~Collisions()
{
    m_instance = nullptr;        // prevent dangling pointer after destruction
}

Collisions* Collisions::Instance()
{
    if (!m_instance)                       // nullptr on first call
    {
        m_instance = new Collisions;     // create the single instance
    }
    return m_instance;                     // always return the same pointer
}

bool Collisions::Circle(GameObject* a, GameObject* b)
{
    Vector2D vec = Vector2D(
        (a->GetPosition().x - b->GetPosition().x),
        (a->GetPosition().y - b->GetPosition().y)
    );

    double distance = sqrt((vec.x * vec.x) + (vec.y * vec.y));

    double combined_distance = (a->GetCollisionRadius() + b->GetCollisionRadius());

    return distance < combined_distance;
}

bool Collisions::Box(GameObject* a, GameObject* b)
{
    Rect2D rect1 = a->GetCollisionBox();
    Rect2D rect2 = b->GetCollisionBox();

    int rect1Left = rect1.x;
    int rect2Left = rect2.x;
    int rect1Top = rect1.y;
    int rect2Top = rect2.y;
    int rect1Bottom = (rect1Top + rect1.h);
    int rect2Bottom = (rect2Top + rect2.h);
    int rect1Right = (rect1Left + rect1.w);
    int rect2Right = (rect2Left + rect2.w);

    return (rect1Left < rect2Right) &&
        (rect1Right > rect2Left) &&
        (rect1Top < rect2Bottom) &&
        (rect1Bottom > rect2Top);
}
