#pragma once
#include "Collider.h"

#define M_PI 3.14159265358979323846

class CircleCollider : public Collider
{
private:
    float m_radius;

public:
    CircleCollider(float _radius, uint32_t _layer = 1, uint32_t _mask = 0xFFFFFFFF) : Collider(ColliderType::Circle, _layer, _mask), m_radius(_radius) {}

    float GetRadius() const { return m_radius; }

    bool IsColliding(Collider* _other) override;

    void Draw() override;

    void SetRadius(float _radius);

    float GetLeft() override;
    float GetRight() override;
    float GetTop() override;
    float GetBottom() override;

    void Inspect() override;
};

