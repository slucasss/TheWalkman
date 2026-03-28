#pragma once

#include <SFML/Window.hpp>
#include <map>
#include "DataAsset.h"
#include "IniFile.h"

class SettingsLoader :public DataAsset {

	static SettingsLoader* Instance;

	IniFile* m_iniFile;

	float m_volume;
	bool m_fullscreen;

public:

	SettingsLoader();

	static SettingsLoader* Get() {
		if (Instance == nullptr) {
			Instance = new SettingsLoader;
		}
		return Instance;
	}

	void Reload() override;

	void Save();

	void SetVolume(float volume);

	void SetFullscreen(bool fullscreen);

	float GetVolume() { return m_volume; }
	bool getFullscreen() { return m_fullscreen; }
 };