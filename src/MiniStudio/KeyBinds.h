#pragma once

#include <SFML/Window.hpp>
#include <map>
#include "DataAsset.h"
#include "IniFile.h"


class KeyBinds: public DataAsset{

    

	static KeyBinds* Instance;
	
	struct bind {
		sf::Keyboard::Key m_keyboard;
		int m_controller;
	};

	std::map<std::string, bind> m_binds;
    std::map<sf::Keyboard::Key, std::string> m_keyToString;
    

	IniFile* m_iniFile;


    const std::map<std::string, sf::Keyboard::Key> keyMap = {
        // Lettres
        {"A", sf::Keyboard::A}, {"B", sf::Keyboard::B}, {"C", sf::Keyboard::C},
        {"D", sf::Keyboard::D}, {"E", sf::Keyboard::E}, {"F", sf::Keyboard::F},
        {"G", sf::Keyboard::G}, {"H", sf::Keyboard::H}, {"I", sf::Keyboard::I},
        {"J", sf::Keyboard::J}, {"K", sf::Keyboard::K}, {"L", sf::Keyboard::L},
        {"M", sf::Keyboard::M}, {"N", sf::Keyboard::N}, {"O", sf::Keyboard::O},
        {"P", sf::Keyboard::P}, {"Q", sf::Keyboard::Q}, {"R", sf::Keyboard::R},
        {"S", sf::Keyboard::S}, {"T", sf::Keyboard::T}, {"U", sf::Keyboard::U},
        {"V", sf::Keyboard::V}, {"W", sf::Keyboard::W}, {"X", sf::Keyboard::X},
        {"Y", sf::Keyboard::Y}, {"Z", sf::Keyboard::Z},

        // Chiffres ligne du haut
        {"Num0", sf::Keyboard::Num0}, {"Num1", sf::Keyboard::Num1},
        {"Num2", sf::Keyboard::Num2}, {"Num3", sf::Keyboard::Num3},
        {"Num4", sf::Keyboard::Num4}, {"Num5", sf::Keyboard::Num5},
        {"Num6", sf::Keyboard::Num6}, {"Num7", sf::Keyboard::Num7},
        {"Num8", sf::Keyboard::Num8}, {"Num9", sf::Keyboard::Num9},

        // Pavé numérique
        {"Numpad0", sf::Keyboard::Numpad0}, {"Numpad1", sf::Keyboard::Numpad1},
        {"Numpad2", sf::Keyboard::Numpad2}, {"Numpad3", sf::Keyboard::Numpad3},
        {"Numpad4", sf::Keyboard::Numpad4}, {"Numpad5", sf::Keyboard::Numpad5},
        {"Numpad6", sf::Keyboard::Numpad6}, {"Numpad7", sf::Keyboard::Numpad7},
        {"Numpad8", sf::Keyboard::Numpad8}, {"Numpad9", sf::Keyboard::Numpad9},

        // Touches de fonction
        {"F1",  sf::Keyboard::F1},  {"F2",  sf::Keyboard::F2},
        {"F3",  sf::Keyboard::F3},  {"F4",  sf::Keyboard::F4},
        {"F5",  sf::Keyboard::F5},  {"F6",  sf::Keyboard::F6},
        {"F7",  sf::Keyboard::F7},  {"F8",  sf::Keyboard::F8},
        {"F9",  sf::Keyboard::F9},  {"F10", sf::Keyboard::F10},
        {"F11", sf::Keyboard::F11}, {"F12", sf::Keyboard::F12},
        {"F13", sf::Keyboard::F13}, {"F14", sf::Keyboard::F14},
        {"F15", sf::Keyboard::F15},

        // Navigation
        {"Up",       sf::Keyboard::Up},       {"Down",  sf::Keyboard::Down},
        {"Left",     sf::Keyboard::Left},     {"Right", sf::Keyboard::Right},
        {"PageUp",   sf::Keyboard::PageUp},   {"PageDown", sf::Keyboard::PageDown},
        {"Home",     sf::Keyboard::Home},     {"End",   sf::Keyboard::End},
        {"Insert",   sf::Keyboard::Insert},   {"Delete", sf::Keyboard::Delete},

        // Modificateurs
        {"LShift",   sf::Keyboard::LShift},  {"RShift",   sf::Keyboard::RShift},
        {"LControl", sf::Keyboard::LControl},{"RControl", sf::Keyboard::RControl},
        {"LAlt",     sf::Keyboard::LAlt},    {"RAlt",     sf::Keyboard::RAlt},
        {"LSystem",  sf::Keyboard::LSystem}, {"RSystem",  sf::Keyboard::RSystem},

        // Touches spéciales
        {"Space",     sf::Keyboard::Space},
        {"Enter",     sf::Keyboard::Enter},
        {"Escape",    sf::Keyboard::Escape},
        {"Backspace", sf::Keyboard::BackSpace},
        {"Tab",       sf::Keyboard::Tab},
        {"Menu",      sf::Keyboard::Menu},

        // Ponctuation / symboles
        {"LBracket",  sf::Keyboard::LBracket},   // [
        {"RBracket",  sf::Keyboard::RBracket},   // ]
        {"Semicolon", sf::Keyboard::SemiColon},  // ;
        {"Comma",     sf::Keyboard::Comma},      // ,
        {"Period",    sf::Keyboard::Period},     // .
        {"Quote",     sf::Keyboard::Quote},      // '
        {"Slash",     sf::Keyboard::Slash},      // /
        {"Backslash", sf::Keyboard::BackSlash},  // '\'
        {"Tilde",     sf::Keyboard::Tilde},      // ~
        {"Equal",     sf::Keyboard::Equal},      // =
        {"Dash",      sf::Keyboard::Dash},       // -
        {"Add",       sf::Keyboard::Add},        // + (pavé num)
        {"Subtract",  sf::Keyboard::Subtract},   // - (pavé num)
        {"Multiply",  sf::Keyboard::Multiply},   // * (pavé num)
        {"Divide",    sf::Keyboard::Divide},     // / (pavé num)

        // Autres
        {"Pause",     sf::Keyboard::Pause},
        {"PrintScreen", sf::Keyboard::Unknown}, // pas supporté nativement en SFML
    };



public:
	KeyBinds();

	static KeyBinds* Get(){
		if (Instance == nullptr) {
			Instance = new KeyBinds;
		}
		return Instance;
	}

	void Reload() override;

	sf::Keyboard::Key GetKeyboardBind(const std::string& name);
	int GetControllerBind(const std::string& name);

    void SetBind(const std::string& key, const std::string& keyboard, int controller);

    void Save();
    std::string KeyToString(sf::Keyboard::Key);
};

