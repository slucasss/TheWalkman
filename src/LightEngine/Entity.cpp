#include "Entity.h"

#include "GameManager.h"
#include "Inspectable.h"
#include "Utils.h"
#include "Debug.h"

void Entity::Destroy()
{
	m_toDestroy = true;
}

uint64_t Entity::s_nextID = 1;

Entity::Entity()
{
	m_id = s_nextID++;
	m_shouldSave = false;
}

Entity::~Entity()
{
	for (Component* c : m_components)
		delete c;
}

void Entity::Update()
{
	auto updatables = GetAllComponents<Updatable>();

	for (Updatable* u : updatables)
	{
		u->Update();
	}
}

void Entity::FixedUpdate()
{
	auto fixedUpdatables = GetAllComponents<FixedUpdatable>();

	for (FixedUpdatable* fu : fixedUpdatables)
	{
		fu->FixedUpdate();
	}
}

void Entity::Draw()
{
	auto drawables = GetAllComponents<Drawable>();

	for (Drawable* d : drawables)
	{
		d->Draw();
	}
}

void Entity::Inspect()
{
	auto inspectables = GetAllComponents<Inspectable>();

	for (Inspectable* i : inspectables)
	{
		if (ImGui::CollapsingHeader(typeid(*i).name()))
		{
			i->Inspect();
		}
	}
}

float Entity::GetDeltaTime() const
{
	return GameManager::Get()->GetDeltaTime();
}

float Entity::GetFixedDeltaTime() const
{
	return GameManager::Get()->GetFixedDeltaTime();
}