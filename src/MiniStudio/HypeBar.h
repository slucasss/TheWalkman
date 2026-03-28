#pragma once
#include "Component.h"
#include "Drawable.h"
#include "Inspectable.h"
#include <SFML/Graphics.hpp>

class HypeBar : public Component, public Drawable, public Inspectable
{
public:
    HypeBar(float width, float height);

    void SetHypeIndicator(float value); // 0 -> 1

    void AddHype(float add);

    void Draw() override;

    void Inspect() override;

    float GetHype() { return m_hype; };

    float m_reTpTimer;

private:
    float m_hype = 50.f;

};

