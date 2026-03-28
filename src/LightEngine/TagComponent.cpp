#include "TagComponent.h"
#include "imgui.h"

void TagComponent::AddTag(const std::string& _tag)
{
	for (std::string tag : m_tags)
	{
		if (tag == _tag)
			return;
	}
	m_tags.push_back(_tag);
}

void TagComponent::RemoveTag(const std::string& _tag)
{
	auto it = std::find(m_tags.begin(), m_tags.end(), _tag);

	if (it != m_tags.end())
		m_tags.erase(it);
}

bool TagComponent::Is(const std::string& _tag) const
{
	for (std::string tag : m_tags)
	{
		if (tag == _tag)
			return true;
	}
	return false;
}

void TagComponent::Inspect()
{
	ImGui::Text("Tags:");

	for (const std::string& tag : m_tags)
	{
		ImGui::BulletText("%s", tag.c_str());
	}
}
