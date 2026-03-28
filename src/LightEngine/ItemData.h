#pragma once
#include "Component.h"
#include <string>

class ItemData : public Component
{
public:
    std::string texturePath;
    std::string dialogue;
};