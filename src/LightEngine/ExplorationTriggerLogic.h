#pragma once
#include "Component.h"
#include "Collidable.h"
#include "Inspectable.h"
#include <functional>
#include <string>

enum TriggerDirection
{
    Any,
    LeftToRight,
    RightToLeft,
    TopToBottom,
    BottomToTop
};

class ExplorationTriggerLogic : public Component, public Collidable, public Inspectable
{
public:

    ExplorationTriggerLogic(const std::string& tag = "Player", TriggerDirection direction = TriggerDirection::Any);

    void SetDirection(TriggerDirection dir);
    TriggerDirection GetDirection() const;

    void SetTargetTag(const std::string& tag);

    void SetOnEnter(std::function<void(CollisionSide)> func);
    void SetOnExit(std::function<void(CollisionSide)> func);

    void OnCollisionEnter(Collider* _self, Collider* _other, CollisionSide side) override;
    void OnCollisionExit(Collider* _self, Collider* _other, CollisionSide side) override;

    void Inspect() override;

private:

    bool CheckDirection(CollisionSide side);
    bool HasValidTag(Collider* other);

    std::string m_targetTag;
    TriggerDirection m_direction;

    std::function<void(CollisionSide)> m_onEnter;
    std::function<void(CollisionSide)> m_onExit;
};