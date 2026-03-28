#pragma once
#include <vector>
#include <list>
#include "Collider.h"

class Entity;

struct CollisionInfo
{
    Collider* a;
    Collider* b;
    CollisionSide sideA;
    CollisionSide sideB;
};

class CollisionSystem
{
private:
    std::vector<CollisionInfo> m_previousCollisions;

public:
    void Update(const std::list<Entity*>& entities);
    void Clear(Entity* entity);
    void ClearAll() { m_previousCollisions.clear(); }
};

