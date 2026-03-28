#pragma once
#include "Collider.h"

class BoxCollider : public Collider
{
private:
	float m_width;
	float m_height;
public:
	BoxCollider(float _width, float _height, uint32_t _layer = 1, uint32_t _mask = 0xFFFFFFFF) : Collider(ColliderType::Rectangle, _layer, _mask), m_width(_width), m_height(_height) {}

    bool IsColliding(Collider* _other) override;

    void Draw() override;

    void SetBox(float _width, float _height);

    float GetWidth() { return m_width; }
    float GetHeight() { return m_height; }

    float GetLeft() override;
    float GetRight() override;
    float GetTop() override;
    float GetBottom() override;

    void Inspect() override;
};

