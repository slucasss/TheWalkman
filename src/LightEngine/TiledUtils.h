#pragma once
#include "tinyxml2.h"
#include <string>

using namespace tinyxml2;

struct TilesetInfo {
    int firstgid;
    std::string source;
};

class TiledUtils
{
    TiledUtils() = default;
public:
    template<typename T>
    static T GetProperty(XMLElement* obj, const std::string& name, const T& defaultValue) = delete;

    static std::string ResolveTiledPath(const std::string& tmxPath, const std::string& relative);
    static std::string ResolveTileGID(const std::string& tmxFilePath, int gid);
};

template<>
inline std::string TiledUtils::GetProperty<std::string>(XMLElement* obj, const std::string& name, const std::string& defaultValue)
{
    if (obj == nullptr) return defaultValue;

    XMLElement* props = obj->FirstChildElement("properties");
    if (!props) return defaultValue;

    for (XMLElement* prop = props->FirstChildElement("property");
        prop != nullptr;
        prop = prop->NextSiblingElement("property"))
    {
        const char* propName = prop->Attribute("name");

        if (propName && name == propName)
        {
            const char* value = prop->Attribute("value");
            if (value)
                return value;
        }
    }

    return defaultValue;
}

template<>
inline int TiledUtils::GetProperty<int>(XMLElement* obj, const std::string& name, const int& defaultValue)
{
    std::string val = TiledUtils::GetProperty<std::string>(obj, name, "");
    if (val.empty()) return defaultValue;

    return std::stoi(val);
}

template<>
inline float TiledUtils::GetProperty<float>(XMLElement* obj, const std::string& name, const float& defaultValue)
{
    std::string val = TiledUtils::GetProperty<std::string>(obj, name, "");
    if (val.empty()) return defaultValue;

    return std::stof(val);
}

template<>
inline bool TiledUtils::GetProperty<bool>(XMLElement* obj, const std::string& name, const bool& defaultValue)
{
    std::string val = TiledUtils::GetProperty<std::string>(obj, name, "");
    if (val.empty()) return defaultValue;

    return val == "true" || val == "1";
}
