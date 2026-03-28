#include "Rigidbody2D.h"
#include "Entity.h"
#include "TransformComponent.h"
#include "Collider.h"
#include "BoxCollider.h"
#include "CircleCollider.h"
#include "Utils.h"
#include "SceneManager.h"
#include "Scene.h"
#include "imgui.h"

Rigidbody2D::Rigidbody2D(float mass, bool useGravity, float restitution)
{
    m_mass = mass;
    m_useGravity = useGravity;
    m_restitution = restitution;
    m_isKinematic = false;

    m_safeMargin = 0.1f;

    m_velocity = { 0, 0 };
    m_acceleration = { 0, 0 };
    m_gravity = { 0, 9.81f };
}

void Rigidbody2D::AddForce(sf::Vector2f force)
{
    m_acceleration = m_acceleration + (force / m_mass);
}

void Rigidbody2D::AddImpulse(sf::Vector2f impulse)
{
    m_velocity = m_velocity + (impulse / m_mass);
}

void Rigidbody2D::SetVelocity(const sf::Vector2f& vel)
{
    m_velocity = vel;
}

sf::Vector2f Rigidbody2D::GetVelocity() const
{
    return m_velocity;
}

void Rigidbody2D::FixedUpdate()
{
    if (m_isKinematic)
        return;

    float deltaTime = m_owner->GetFixedDeltaTime();

    if (m_useGravity)
        m_acceleration = m_acceleration + sf::Vector2f(m_gravity.x * PIXEL_PER_METER, m_gravity.y * PIXEL_PER_METER);

    m_velocity = m_velocity + (m_acceleration * deltaTime);

    TransformComponent* transform = m_owner->GetComponent<TransformComponent>();
    if (transform)
    {
        sf::Vector2f newPos = transform->GetPosition() + (m_velocity * deltaTime);
        transform->SetPosition(newPos);
    }

    m_acceleration = sf::Vector2f(0, 0);
}

void Rigidbody2D::OnCollisionStay(Collider* _self, Collider* _other, CollisionSide side)
{
    if (_self->IsTrigger() || _other->IsTrigger())
        return;

    Rigidbody2D* otherRb = _other->GetOwner()->GetComponent<Rigidbody2D>();

    TransformComponent* selfTransform = _self->GetOwner()->GetComponent<TransformComponent>();
    TransformComponent* otherTransform = _other->GetOwner()->GetComponent<TransformComponent>();

    sf::Vector2f selfTransformPos = selfTransform->GetPosition();
    sf::Vector2f otherTransformPos = otherTransform->GetPosition();

    sf::Vector2f selfPos = selfTransformPos + _self->GetOffset();
    sf::Vector2f otherPos = otherTransformPos + _other->GetOffset();

    sf::Vector2f normal;
    float penetrationDepth = 0.0f;

    ColliderType selfType = _self->GetType();
    ColliderType otherType = _other->GetType();

    // ----------- Calcul du vecteur normal selon le type de collision -----------
    if (selfType == ColliderType::Rectangle && otherType == ColliderType::Rectangle)
    {
        BoxCollider* a = static_cast<BoxCollider*>(_self);
        BoxCollider* b = static_cast<BoxCollider*>(_other);

        float dx = selfPos.x - otherPos.x;
        float dy = selfPos.y - otherPos.y;

        float overlapX = (a->GetWidth() * 0.5f + b->GetWidth() * 0.5f) - std::abs(dx);
        float overlapY = (a->GetHeight() * 0.5f + b->GetHeight() * 0.5f) - std::abs(dy);

        if (overlapX < overlapY)
        {
            normal = (dx < 0) ? sf::Vector2f(-1, 0) : sf::Vector2f(1, 0);
            penetrationDepth = overlapX;
        }
        else
        {
            normal = (dy < 0) ? sf::Vector2f(0, -1) : sf::Vector2f(0, 1);
            penetrationDepth = overlapY;
        }
    }
    else if (selfType == ColliderType::Circle && otherType == ColliderType::Circle)
    {
        CircleCollider* a = static_cast<CircleCollider*>(_self);
        CircleCollider* b = static_cast<CircleCollider*>(_other);

        sf::Vector2f delta = selfPos - otherPos;
        float dist = Utils::GetDistance(otherPos, selfPos);
        float r = a->GetRadius() + b->GetRadius();

        if (dist != 0)
            normal = delta / dist;
        else
            normal = sf::Vector2f(0, -1);

        penetrationDepth = r - dist;
    }
    else
    {
        // Box <-> Circle
        BoxCollider* box = (selfType == ColliderType::Rectangle ? static_cast<BoxCollider*>(_self) : static_cast<BoxCollider*>(_other));
        CircleCollider* circle = (selfType == ColliderType::Circle ? static_cast<CircleCollider*>(_self) : static_cast<CircleCollider*>(_other));

        sf::Vector2f boxPos = box->GetOwner()->GetComponent<TransformComponent>()->GetPosition() + box->GetOffset();
        sf::Vector2f circlePos = circle->GetOwner()->GetComponent<TransformComponent>()->GetPosition() + circle->GetOffset();

        float halfW = box->GetWidth() * 0.5f;
        float halfH = box->GetHeight() * 0.5f;

        // Clamp du centre du cercle au rectangle
        float clampedX = std::max(boxPos.x - halfW,
            std::min(circlePos.x, boxPos.x + halfW));
        float clampedY = std::max(boxPos.y - halfH,
            std::min(circlePos.y, boxPos.y + halfH));

        sf::Vector2f closest(clampedX, clampedY);
        sf::Vector2f delta = circlePos - closest;

        float dist = Utils::GetDistance(closest, circlePos);

        if (dist > 0.0001f)
        {
            normal = delta / dist;
            penetrationDepth = circle->GetRadius() - dist;
        }
        else
        {
            float dx = circlePos.x - boxPos.x;
            float dy = circlePos.y - boxPos.y;

            float overlapX = halfW - std::abs(dx);
            float overlapY = halfH - std::abs(dy);

            if (overlapX < overlapY)
            {
                normal = (dx < 0) ? sf::Vector2f(-1, 0) : sf::Vector2f(1, 0);
                penetrationDepth = circle->GetRadius() + overlapX;
            }
            else
            {
                normal = (dy < 0) ? sf::Vector2f(0, -1) : sf::Vector2f(0, 1);
                penetrationDepth = circle->GetRadius() + overlapY;
            }
        }

        if (selfType == ColliderType::Rectangle)
            normal = normal * -1.f;
    }

    if (penetrationDepth <= 0)
        return;

    // ---------------- SAFE MARGIN + POSITION CORRECTION ----------------
    float invMassA = m_isKinematic ? 0.0f : 1.0f / m_mass;
    float invMassB = (otherRb && !otherRb->m_isKinematic) ? 1.0f / otherRb->m_mass : 0.0f;

    float correctionPercent = 1.f;
    float slop = m_safeMargin;

    float correctionMag = std::max(penetrationDepth - slop, 0.0f) / (invMassA + invMassB) * correctionPercent;
    sf::Vector2f correction = normal * correctionMag;

    if (!m_isKinematic)
        selfTransform->SetPosition(selfTransformPos + correction * invMassA);

    if (otherRb && !otherRb->m_isKinematic)
        otherTransform->SetPosition(otherTransformPos - correction * invMassB);

    // ----------- Calcul de la vélocité relative -----------
    sf::Vector2f otherVelocity = otherRb ? otherRb->m_velocity : sf::Vector2f(0, 0);
    sf::Vector2f relativeVelocity = m_velocity - otherVelocity;

    float velAlongNormal = Utils::Dot(relativeVelocity, normal);

    if (velAlongNormal > 0)
        return;

    // ----------- Calcul de l'impulsion -----------

    float restitution = otherRb ? std::min(m_restitution, otherRb->m_restitution) : m_restitution;

    float j = -(1 + restitution) * velAlongNormal / (invMassA + invMassB);
    sf::Vector2f impulse = normal * j;

    // ----------- Applique l'impulsion -----------
    AddImpulse(impulse);

    if (otherRb && !otherRb->m_isKinematic)
    {
        sf::Vector2f ref{ 0, 0 };
        otherRb->AddImpulse(ref - impulse);
    }
}

void Rigidbody2D::Inspect()
{
    ImGui::Checkbox("UseGravity", &m_useGravity);
    ImGui::Checkbox("Kinematic", &m_isKinematic);
    ImGui::DragFloat("Mass", &m_mass, 0.05f);
    ImGui::DragFloat("Restitution", &m_restitution, 0.001f);
    ImGui::DragFloat("SafeMargin", &m_safeMargin, 0.0001f);

    ImGui::DragFloat2("Velocity", &m_velocity.x);
    ImGui::DragFloat2("Acceleration", &m_acceleration.x);
    ImGui::DragFloat2("GravityForce", &m_gravity.x);
}
