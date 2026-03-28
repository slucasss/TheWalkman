#pragma once
#include "Component.h"

class RenderLayerComponent : public Component
{
public:
    RenderLayerComponent(int layer = 0) : m_layer(layer) {}

    int GetLayer() const { return m_layer; }
    void SetLayer(int layer) { m_layer = layer; }

private:
    int m_layer;
};