#pragma once

class Entity;

class Component
{
protected:
	Entity* m_owner;

public:
	Component();
	virtual ~Component() = default;

	void SetOwner(Entity* _owner);
	Entity* GetOwner();
};

