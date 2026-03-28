#pragma once
#include "Drawable.h"
#include "imgui.h" 
#include <string>
#include <vector>

class StatField;
class Entity;

class StatsWindow : public Drawable
{
    std::string m_name;
    std::vector<StatField*> m_fields;
    Entity* m_targetEntity;

public:
    StatsWindow(const std::string& name) : m_name(name), m_targetEntity(nullptr) {}

    void AddFloat(const std::string& name, float* value, float min, float max);

    void Inspect(Entity* targetEntity) { m_targetEntity = targetEntity; }

    void Draw() override;
};

