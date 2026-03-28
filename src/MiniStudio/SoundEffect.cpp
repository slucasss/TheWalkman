#include "SoundEffect.h"
#include "Entity.h"
#include "imgui.h"

void SoundEffect::PlaySoundEffect()
{
	if (m_timer <= 0) {
		this->Play();
		m_timer = m_cooldown;
	}
	this->SetVolume(m_volume);

	m_timer -= m_owner->GetDeltaTime();

}

void SoundEffect::StopSoundEffect()
{
	m_timer = 0;
	this->Stop();
}

void SoundEffect::SetCooldown(float cooldown)
{
	m_cooldown = cooldown;
}

void SoundEffect::Inspect()
{
	ImGui::DragFloat("volume", &m_volume, 0.05f);
	ImGui::DragFloat("cooldown", &m_cooldown, 0.05f);
}
