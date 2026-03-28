#pragma once
#include <string>
#include <unordered_map>

class IniFile
{
    std::string m_path;
    std::unordered_map<std::string, std::unordered_map<std::string, float>> m_data;
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> m_dataString;

public:
    void InitPath(const std::string& path);

    bool Load();
    bool Save() const;

    void Open();

    void SetFloat(const std::string& section, const std::string& key, float value);

    void SetString(const std::string& section, const std::string& key, std::string value);

    float GetFloat(const std::string& section, const std::string& key, float defaultValue) const;

    std::string GetString(const std::string& section, const std::string& key, std::string defaultValue) const;

    std::string GetPath() { return m_path; }
};
