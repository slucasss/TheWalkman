#include "KeyBinds.h"

#include "DataManager.h"
#include "FileDataSource.h"

KeyBinds* KeyBinds::Instance = nullptr;

KeyBinds::KeyBinds() {

    m_iniFile = new IniFile();
    m_iniFile->InitPath("../../../src/config/Keybinds.ini");
    m_iniFile->Load();
    Reload();

    FileDataSource* filedata = new FileDataSource("../../../src/config/Keybinds.ini");
    DataManager::Get().Register(this, filedata);
    
    for (const auto& pair : keyMap)
        m_keyToString[pair.second] = pair.first;
}

void KeyBinds::Reload(){
    m_iniFile->Load();
    {
        std::string keyCode;

        keyCode = m_iniFile->GetString("Keybinds", "GoRight", "D");

        m_binds["GoRight"] = bind{keyMap.at(keyCode), -1};
    }
    {
        std::string keyCode;

        keyCode = m_iniFile->GetString("Keybinds", "GoLeft", "Q");


        m_binds["GoLeft"] = bind{ keyMap.at(keyCode), -1};
    }
    {
        std::string keyCode;
        int controllerCode;

        keyCode = m_iniFile->GetString("Keybinds", "Run", "LShift");
		controllerCode = m_iniFile->GetFloat("Controller", "Run", 10);


        m_binds["Run"] = bind{ keyMap.at(keyCode), -1};
    }
    {
        std::string keyCode;
        int controllerCode;

        keyCode = m_iniFile->GetString("Keybinds", "Jump", "Space");
        controllerCode = m_iniFile->GetFloat("Controller", "Jump", 1);

        m_binds["Jump"] = bind{ keyMap.at(keyCode), controllerCode };
    }
    {
        std::string keyCode;
        int controllerCode;

        keyCode = m_iniFile->GetString("Keybinds", "Pause", "Escape");
        controllerCode = m_iniFile->GetFloat("Controller", "Pause", 9);
    {
        std::string keyCode;

        keyCode = m_iniFile->GetString("Keybinds", "Left_Arrow", "Left");

        m_binds["Left_Arrow"] = bind{ keyMap.at(keyCode), -1 };
	}

	{
		std::string keyCode;

		keyCode = m_iniFile->GetString("Keybinds", "Right_Arrow", "Right");

		m_binds["Right_Arrow"] = bind{ keyMap.at(keyCode), -1 };
	}

    {
        std::string keyCode;

        keyCode = m_iniFile->GetString("Keybinds", "Down_Arrow", "Down");

        m_binds["Down_Arrow"] = bind{ keyMap.at(keyCode), -1 };
    }

    {
        std::string keyCode;

        keyCode = m_iniFile->GetString("Keybinds", "Up_Arrow", "Up");

        m_binds["Up_Arrow"] = bind{ keyMap.at(keyCode), -1 };
    }


        m_binds["Pause"] = bind{ keyMap.at(keyCode), controllerCode };
    }
    {
		std::string keyCode;
		int controllerCode;

		keyCode = m_iniFile->GetString("Keybinds", "Interact", "T");
		controllerCode = m_iniFile->GetFloat("Controller", "Interact", 7);

        m_binds["Interact"] = bind{ keyMap.at(keyCode), controllerCode };
    }
}




sf::Keyboard::Key KeyBinds::GetKeyboardBind(const std::string& name){

    return m_binds[name].m_keyboard;
}

int KeyBinds::GetControllerBind(const std::string& name){

    return m_binds[name].m_controller;
}

//"" = don't change keyboard, -1 = don't change controller
void KeyBinds::SetBind(const std::string& key, const std::string& keyboard, int controller){
    if (keyboard != "") {
        m_iniFile->SetString("Keybinds", key, keyboard);
    }
    if (controller != -1) {
        m_iniFile->SetFloat("Controller", key, controller);

    }
}

void KeyBinds::Save(){
    m_iniFile->Save();
}

std::string KeyBinds::KeyToString(sf::Keyboard::Key key) {
    return m_keyToString[key];
}