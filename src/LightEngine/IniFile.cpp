#include "IniFile.h"
#include <fstream>
#include <iostream>

void IniFile::InitPath(const std::string& path)
{
    m_path = path;
}

bool IniFile::Load()
{
    std::ifstream file(m_path);
    if (file.is_open() == false)
        return false;

    m_data.clear();

    std::string line;
    std::string currentSection;

    while (std::getline(file, line))
    {
        if (line.empty() || line[0] == ';' || line[0] == '#')
            continue;

        if (line.front() == '[' && line.back() == ']')
        {
            currentSection = line.substr(1, line.size() - 2);
            continue;
        }

        size_t sep = line.find('=');
        if (sep == std::string::npos)
            continue;

        std::string key = line.substr(0, sep);
        std::string valueStr = line.substr(sep + 1);


        key.erase(key.find_last_not_of(" \t") + 1);
        valueStr.erase(0, valueStr.find_first_not_of(" \t"));

        if (valueStr.front() == '"' && valueStr.back() == '"')
        {
            m_dataString[currentSection][key] = valueStr.substr(1, valueStr.size() - 2);
            continue;
        }

        try
        {
            float value = std::stof(valueStr);
            m_data[currentSection][key] = value;
        }
        catch (const std::exception& e)
        {
            std::cout << "Invalid value in ini: " << key << std::endl;
        }
    }

    return true;
}

bool IniFile::Save() const
{
    std::ofstream file(m_path);
    if (file.is_open() == false)
        return false;

    for (const auto& sectionPair : m_data)
    {
        const std::string& sectionName = sectionPair.first;
        const auto& keys = sectionPair.second;

        file << "[" << sectionName << "]\n";

        for (const auto& keyPair : keys)
        {
            file << keyPair.first << "=" << keyPair.second << "\n";
        }

        file << "\n";
    }

    for (const auto& sectionPair : m_dataString)
    {
        const std::string& sectionName = sectionPair.first;
        const auto& keys = sectionPair.second;

        file << "[" << sectionName << "]\n";

        for (const auto& keyPair : keys)
        {
            file << keyPair.first << "=" << "\"" << keyPair.second << "\"" << "\n";
        }

        file << "\n";
    }


    file.close();
    return true;
}

void IniFile::Open()
{
#if defined(_WIN32)
    std::string command = "start \"\" \"" + m_path + "\"";
#elif defined(__APPLE__)
    std::string command = "open \"" + m_path + "\"";
#elif defined(__linux__)
    std::string command = "xdg-open \"" + m_path + "\"";
#else
    std::cerr << "OS non supporté pour ouvrir le fichier." << std::endl;
    return;
#endif

    system(command.c_str());
}

float IniFile::GetFloat(const std::string& section, const std::string& key, float defaultValue) const
{
    auto secIt = m_data.find(section);
    if (secIt == m_data.end())
        return defaultValue;

    auto keyIt = secIt->second.find(key);
    if (keyIt == secIt->second.end())
        return defaultValue;

    return keyIt->second;
}

std::string IniFile::GetString(const std::string& section, const std::string& key, std::string defaultValue) const
{
    auto secIt = m_dataString.find(section);
    if (secIt == m_dataString.end())
        return defaultValue;

    auto keyIt = secIt->second.find(key);
    if (keyIt == secIt->second.end())
        return defaultValue;

    return keyIt->second;
}

void IniFile::SetFloat(const std::string& section, const std::string& key, float value)
{
    m_data[section][key] = value;
}

void IniFile::SetString(const std::string& section, const std::string& key, std::string value)
{
    m_dataString[section][key] = value;
}



