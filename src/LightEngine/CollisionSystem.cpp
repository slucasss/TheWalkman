#include "CollisionSystem.h"
#include "Entity.h"
#include "Collider.h"
#include "Collidable.h"

bool SamePair(const CollisionInfo& c1, Collider* a, Collider* b)
{
    return (c1.a == a && c1.b == b) || (c1.a == b && c1.b == a);
}

bool WasColliding(const std::vector<CollisionInfo>& list, Collider* a, Collider* b)
{
    for (const CollisionInfo& c : list)
    {
        if (SamePair(c, a, b))
            return true;
    }

    return false;
}

CollisionSide GetCollisionSide(Collider* a, Collider* b)
{
    float overlapLeft = a->GetRight() - b->GetLeft();
    float overlapRight = b->GetRight() - a->GetLeft();
    float overlapTop = a->GetBottom() - b->GetTop();
    float overlapBottom = b->GetBottom() - a->GetTop();

    float minOverlap = std::min(std::min(overlapLeft, overlapRight), std::min(overlapTop, overlapBottom));

    if (minOverlap == overlapLeft)
        return CollisionSide::Right;

    if (minOverlap == overlapRight)
        return CollisionSide::Left;

    if (minOverlap == overlapTop)
        return CollisionSide::Bottom;

    if (minOverlap == overlapBottom)
        return CollisionSide::Top;

    return CollisionSide::None;
}

void CollisionSystem::Update(const std::list<Entity*>& entities)
{
    std::vector<Collider*> colliders;
    std::vector<CollisionInfo> currentCollisions;

    for (Entity* e : entities)
    {
        auto cols = e->GetAllComponents<Collider>();
        colliders.insert(colliders.end(), cols.begin(), cols.end());
    }

    for (int i = 0; i < colliders.size(); ++i)
    {
        for (int j = i + 1; j < colliders.size(); ++j)
        {
            Collider* a = colliders[i];
            Collider* b = colliders[j];

            if (a->GetOwner() == b->GetOwner())
                continue;

            if (a->CanCollide(b) == false)
                continue;

            if (a->IsColliding(b) == false)
                continue;

            CollisionSide sideA = GetCollisionSide(a, b);
            CollisionSide sideB = GetCollisionSide(b, a);

            currentCollisions.push_back({ a, b, sideA, sideB });

            bool wasColliding = WasColliding(m_previousCollisions, a, b);

            auto aListeners = a->GetOwner()->GetAllComponents<Collidable>();
            auto bListeners = b->GetOwner()->GetAllComponents<Collidable>();

            if (wasColliding == false)
            {
                for (Collidable* c : aListeners)
                    c->OnCollisionEnter(a, b, sideA);

                for (Collidable* c : bListeners)
                    c->OnCollisionEnter(b, a, sideB);
            }

            for (Collidable* c : aListeners)
                c->OnCollisionStay(a, b, sideA);

            for (Collidable* c : bListeners)
                c->OnCollisionStay(b, a, sideB);
        }
    }

    for (const CollisionInfo& oldCol : m_previousCollisions)
    {
        if (WasColliding(currentCollisions, oldCol.a, oldCol.b) == false)
        {
            auto aListeners = oldCol.a->GetOwner()->GetAllComponents<Collidable>();
            auto bListeners = oldCol.b->GetOwner()->GetAllComponents<Collidable>();

            for (Collidable* c : aListeners)
                c->OnCollisionExit(oldCol.a, oldCol.b, oldCol.sideA);

            for (Collidable* c : bListeners)
                c->OnCollisionExit(oldCol.b, oldCol.a, oldCol.sideB);
        }
    }

    m_previousCollisions = currentCollisions;
}

void CollisionSystem::Clear(Entity* entity)
{
    std::vector<Collider*> colliders = entity->GetAllComponents<Collider>();

    m_previousCollisions.erase(
        std::remove_if(
            m_previousCollisions.begin(),
            m_previousCollisions.end(),
            [&colliders](const CollisionInfo& info)
            {
                for (Collider* col : colliders)
                {
                    if (info.a == col || info.b == col)
                        return true;
                }
                return false;
            }
        ),
        m_previousCollisions.end()
    );
}

