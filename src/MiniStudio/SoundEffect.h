#pragma once
#include "Sound.h"
#include "Inspectable.h"


class SoundEffect : public Sound, public Inspectable
{
	float m_timer = 0.f;
	float m_cooldown = 0.5f;
	float m_volume = 50.f;
public:
	void PlaySoundEffect();

	void StopSoundEffect();

	void SetCooldown(float cooldown);

	void Inspect() override;
};

