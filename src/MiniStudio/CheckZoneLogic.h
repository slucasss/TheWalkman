#pragma once
#include "Component.h"
#include "Updatable.h"
#include "Collidable.h"
#include "Scene.h"

class CheckZoneLogic : public Component, public Collidable, public Updatable
{
    bool m_isPlaying;

    bool m_isRightArrowInZone;
    bool m_isLeftArrowInZone;
    bool m_isUpArrowInZone;
    bool m_isDownArrowInZone;

    //int m_drunkLevel = 0;

    float m_dist = -1.f;
    sf::Vector2f m_arrowPos;


public:
    CheckZoneLogic();

    void Update() override;

    void OnCollisionStay(Collider* _self, Collider* _other, CollisionSide side) override;
    void OnCollisionExit(Collider* _self, Collider* _other, CollisionSide side) override;
};
