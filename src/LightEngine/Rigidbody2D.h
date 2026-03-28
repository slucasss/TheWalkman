#pragma once
#include "Component.h"
#include "FixedUpdatable.h"
#include "Collidable.h"
#include "Inspectable.h"
#include <SFML/System/Vector2.hpp>

#define PIXEL_PER_METER 100

class Collider;

class Rigidbody2D : public Component, public FixedUpdatable, public Collidable, public Inspectable
{
public:
    Rigidbody2D(float mass = 1.0f, bool useGravity = true, float restitution = 0.8f);

    void AddForce(sf::Vector2f force);
    void AddImpulse(sf::Vector2f impulse);

    void SetVelocity(const sf::Vector2f& vel);
    sf::Vector2f GetVelocity() const;

    void SetKinematic(bool _value) { m_isKinematic = _value; }
    bool IsKinematic() { return m_isKinematic; }
    void SetSafeMargin(float _safeMargin) { m_safeMargin = _safeMargin; }
    void SetGravity(sf::Vector2f _g) { m_gravity = _g; }
    void SetMass(float _mass) { m_mass = _mass; }
    void SetRestitution(float _rest) { m_restitution = _rest; }

    void FixedUpdate() override;

    void OnCollisionStay(Collider* _self, Collider* _other, CollisionSide side) override;

    void Inspect() override;

private:
    float m_mass;
    bool m_useGravity;
    float m_restitution;
    bool m_isKinematic;

    float m_safeMargin;

    sf::Vector2f m_velocity;
    sf::Vector2f m_acceleration;
    sf::Vector2f m_gravity;
};
