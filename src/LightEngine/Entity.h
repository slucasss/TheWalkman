#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include "Updatable.h"
#include "FixedUpdatable.h"
#include "Drawable.h"
#include "Component.h"

class Scene;

class Entity : public Updatable, public FixedUpdatable, public Drawable
{
	std::vector<Component*> m_components;

	bool m_toDestroy;

	uint64_t m_id;
	static uint64_t s_nextID;

	bool m_shouldSave;

	void Inspect();

public:
	void Update() override;
	void FixedUpdate() override;
	void Draw() override;

    void Destroy();
	bool ToDestroy() const { return m_toDestroy; }

	uint64_t GetID() const { return m_id; }

	void SetSavable(bool value) { m_shouldSave = value; }
	bool ShouldSave() { return m_shouldSave; }

	float GetDeltaTime() const;
	float GetFixedDeltaTime() const;

	template<typename T, typename... Args>
	T* AddComponent(Args&& ...args)
	{
		static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");

		T* component = new T(std::forward<Args>(args)...);
		m_components.push_back(component);
		component->SetOwner(this);
		return component;
	}

	template<typename T>
	T* GetComponent()
	{
		for (Component* c : m_components)
		{
			if (auto cast = dynamic_cast<T*>(c))
				return cast;
		}

		return nullptr;
	}

	template<typename T>
	std::vector<T*> GetAllComponents()
	{
		std::vector<T*> result;

		for (Component* c : m_components)
		{
			if (auto cast = dynamic_cast<T*>(c))
				result.push_back(cast);
		}

		return result;
	}

    Entity();
    ~Entity();
	
private:

    friend class GameManager;
    friend Scene;
	friend class StatsWindow;
};
