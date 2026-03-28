#pragma once

#include "../../include/json.hpp"

using json = nlohmann::json;

class Savable
{
public:

	virtual json Save() = 0;
	virtual void Load(const json& data) = 0;

	virtual const char* GetSaveName() const = 0;

	virtual ~Savable() = default;
};