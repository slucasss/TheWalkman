#pragma once
#include "Component.h"
#include "Inspectable.h"
#include <string>
#include <vector>

class TagComponent : public Component, public Inspectable
{
    std::vector<std::string> m_tags;

public:
    void AddTag(const std::string& _tag);
    void RemoveTag(const std::string& _tag);

    bool Is(const std::string& _tag) const;
    void Inspect() override;
};