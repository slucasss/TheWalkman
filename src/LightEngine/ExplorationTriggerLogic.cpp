#include "ExplorationTriggerLogic.h"
#include "TagComponent.h"
#include "Entity.h"
#include "Collider.h"
#include "imgui.h"

ExplorationTriggerLogic::ExplorationTriggerLogic(const std::string& tag, TriggerDirection direction)
{
    m_targetTag = tag;
    m_direction = direction;
}

void ExplorationTriggerLogic::SetDirection(TriggerDirection dir)
{
    m_direction = dir;
}

TriggerDirection ExplorationTriggerLogic::GetDirection() const
{
    return m_direction;
}

void ExplorationTriggerLogic::SetTargetTag(const std::string& tag)
{
    m_targetTag = tag;
}

void ExplorationTriggerLogic::SetOnEnter(std::function<void(CollisionSide)> func)
{
    m_onEnter = func;
}

void ExplorationTriggerLogic::SetOnExit(std::function<void(CollisionSide)> func)
{
    m_onExit = func;
}

bool ExplorationTriggerLogic::HasValidTag(Collider* other)
{
    auto tagComp = other->GetOwner()->GetComponent<TagComponent>();

    return tagComp->Is(m_targetTag);
}

bool ExplorationTriggerLogic::CheckDirection(CollisionSide side)
{
    switch (m_direction)
    {
    case TriggerDirection::Any:
        return true;

    case TriggerDirection::LeftToRight:
        return side == CollisionSide::Right;

    case TriggerDirection::RightToLeft:
        return side == CollisionSide::Left;

    case TriggerDirection::TopToBottom:
        return side == CollisionSide::Bottom;

    case TriggerDirection::BottomToTop:
        return side == CollisionSide::Top;
    }

    return false;
}

void ExplorationTriggerLogic::OnCollisionEnter(Collider* _self, Collider* _other, CollisionSide side)
{
    if (HasValidTag(_other) == false)
        return;

    if (CheckDirection(side) == false)
        return;

    if (m_onEnter)
        m_onEnter(side);
}

void ExplorationTriggerLogic::OnCollisionExit(Collider* _self, Collider* _other, CollisionSide side)
{
    if (HasValidTag(_other) == false)
        return;

    if (CheckDirection(side) == false)
        return;

    if (m_onExit)
        m_onExit(side);
}

void ExplorationTriggerLogic::Inspect()
{
    ImGui::Text("Exploration Trigger");

    ImGui::Text("Target Tag: %s", m_targetTag.c_str());

    const char* dirs[] =
    {
        "Any",
        "LeftToRight",
        "RightToLeft",
        "TopToBottom",
        "BottomToTop"
    };

    int dir = (int)m_direction;

    if (ImGui::Combo("Direction", &dir, dirs, 5))
        m_direction = (TriggerDirection)dir;
}