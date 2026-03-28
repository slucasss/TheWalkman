#pragma once
#include "StatField.h"
#include <string>
#include "imgui.h"

class FloatField : public StatField
{
    std::string m_name;
    float* m_value;
    float m_min;
    float m_max;

public:
    FloatField(const std::string& name, float* value, float min, float max) : m_name(name), m_value(value), m_min(min), m_max(max) {}

    void Draw() override;
};

