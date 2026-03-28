#pragma once
#include "Component.h"
#include "Drawable.h"
#include "Inspectable.h"
#include <SFML/System/Vector2.hpp>
#include <string>
#include <vector>

enum ColliderType
{
    Rectangle,
    Circle
};

enum CollisionSide
{
    None,
    Left,
    Right,
    Top,
    Bottom
};

class Collider : public Component, public Drawable, public Inspectable
{
protected:
    float m_offsetX;
    float m_offsetY;
    ColliderType m_type;

    bool m_isActived;
    bool m_isVisible = true;
    bool m_isTrigger;

    uint32_t m_layer;
    uint32_t m_mask;

public:
    Collider(ColliderType _type, uint32_t _layer = 1, uint32_t _mask = 0xFFFFFFFF);
    virtual ~Collider() {}

    ColliderType GetType() const { return m_type; }

    void SetLayer(uint32_t _layer) { m_layer = _layer; }
    void SetMask(uint32_t _mask) { m_mask = _mask; }
    bool CanCollide(Collider* _other);

    void SetOffset(float _ox, float _oy);
    sf::Vector2f GetOffset() { return sf::Vector2f(m_offsetX, m_offsetY); }

    void SetActive(bool _value) { m_isActived = _value; }
    bool IsActived() { return m_isActived; }

    void SetVisible(bool _value) { m_isVisible = _value; }
    bool IsVisible() { return m_isVisible; }

    void SetTrigger(bool _value) { m_isTrigger = _value; }
    bool IsTrigger() { return m_isTrigger; }

    uint32_t GetMask() { return m_mask; }

    virtual bool IsColliding(Collider* other) = 0;

    virtual float GetLeft() = 0;
    virtual float GetRight() = 0;
    virtual float GetTop() = 0;
    virtual float GetBottom() = 0;

    virtual void Inspect() override;
};

