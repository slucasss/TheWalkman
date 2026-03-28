#pragma once
#include "Component.h"
#include "Updatable.h"
#include "Drawable.h"
#include <functional>

#include "SFML/System.hpp"



class Button: public Component, public Updatable, public Drawable {

protected:
	sf::Vector2f m_size;

	std::function<void()> m_OnClick;
	std::function<void()> m_MouseOn;

	std::function<void(Entity*)>m_onClickWithArg;
	Entity* m_arg = nullptr;

	bool m_visible = false;
	
	bool m_isControllerSelected = false;

	std::string m_path = "";
	std::string m_selectedPath = "";

	bool m_activate = true;
public:
	Button(){};

	virtual void Update() override;
	virtual void Draw() override;


	void SetMouseOn(std::function<void()> func) { 
		m_MouseOn = func; 
	}
	void SetOnClick(std::function<void()> func) { 
		m_OnClick = func; 
	}
	void SetOnClickWithArg(std::function<void(Entity*)> func) {
		m_onClickWithArg = func;
	}
	void SetSize(sf::Vector2f size) { 
		m_size = size; 
	}

	void SetIsSelected(bool isSelected) { m_isControllerSelected = isSelected; }
	bool IsSelected() { return m_isControllerSelected; }

	void SetPath(std::string path);
	void SetSelectedPath(std::string path);

	void SetActivate(bool ac) { m_activate = ac; }
};

