#pragma once
#include "Component.h"
#include "Drawable.h"
#include "Updatable.h"
#include "Inspectable.h"

#include <map>
#include <vector>

class Animation : public Component, public Updatable, public Inspectable
{

	struct Anim{
		std::vector<int> frames;
		bool loop;
		float frameTime;
	};
	
	int m_width;
	int m_height;

	std::map<std::string, Anim> m_animations;

	int m_currentFrame = 0;
	std::string m_currentAnimation;

	float m_timer;
	bool m_end;

public:
	Animation(std::string path);

	void Update() override;

	//Set the animation with it name
	void SetAnimation(std::string name);

	bool IsAnimationEnded();
	std::string AnimationIs();

	void Inspect() override;
};

