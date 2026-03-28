#include "SettingsLoader.h"

#include "DataManager.h"
#include "FileDataSource.h"
#include "GameManager.h"

#include "SFML/audio.hpp"

SettingsLoader* SettingsLoader::Instance = nullptr;

SettingsLoader::SettingsLoader(){

    m_iniFile = new IniFile();
    m_iniFile->InitPath("../../../src/config/Keybinds.ini");
    m_iniFile->Load();
    Reload();

    FileDataSource* filedata = new FileDataSource("../../../src/config/Keybinds.ini");
    DataManager::Get().Register(this, filedata);

    
}

void SettingsLoader::Reload(){
    m_iniFile->Load();

    m_volume = m_iniFile->GetFloat("Global Settings", "volume", 50);
    m_fullscreen = static_cast<bool>(m_iniFile->GetFloat("Global Settings", "fullscreen", 0));

    sf::Listener::setGlobalVolume(m_volume);
}

void SettingsLoader::Save() {
    m_iniFile->Save();
}

void SettingsLoader::SetVolume(float volume) {
    m_iniFile->SetFloat("Global Settings", "Volume", volume);
}

void SettingsLoader::SetFullscreen(bool fullscreen) {
    m_iniFile->SetFloat("Global Settings", "Fullscreen", fullscreen);
}