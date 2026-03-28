#include "Component.h"

Component::Component()
{
	m_owner = nullptr;
}

void Component::SetOwner(Entity* _owner)
{
	m_owner = _owner;
}

Entity* Component::GetOwner()
{
	return m_owner;
}

