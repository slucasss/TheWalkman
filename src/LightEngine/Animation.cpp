#include "Animation.h"
#include "Sprite.h"
#include "fstream"
#include "json.hpp"
#include "imgui.h"

using json = nlohmann::json;

Animation::Animation(std::string path): m_end(true), m_timer(0) {

    std::ifstream file(path);
    json data;
    file >> data;

    if(data.contains("frame_size"))
    {
        m_width = data["frame_size"]["width"];
        m_height = data["frame_size"]["height"];
    }

    if (data.contains("animations"))
    {
        for (auto& [name, anim] : data["animations"].items())
        {
            Anim a;

            a.frames = anim["frames"].get<std::vector<int>>();
            a.loop = anim["loop"];
            a.frameTime = anim["frame_time"];

            m_animations[name] = a;
        }
    }
}

void Animation::Update() {
    if (m_end == true) return;

    m_timer += m_owner->GetDeltaTime();

    Anim& anim = m_animations[m_currentAnimation];
    if (m_timer >= anim.frameTime)
    {
        m_timer -= anim.frameTime;

        Sprite* sprite = m_owner->GetComponent<Sprite>();
        int frameIndex = anim.frames[m_currentFrame];
        int columns = sprite->GetWidth() / m_width;
        int x = frameIndex % columns;
        int y = frameIndex / columns;
        sprite->SetSrcRect(x * m_width, y * m_height, m_width, m_height);

        m_currentFrame++;
        if (m_currentFrame >= (int)anim.frames.size())
        {
            if (!anim.loop)
            {
                m_end = true;
                return;
            }
            m_currentFrame = 0;
        }
    }
}

void Animation::SetAnimation(std::string name){
	if (m_animations.find(name) != m_animations.end()) {

        if (m_currentAnimation == name && m_end == false) 
            return;

		m_currentFrame = 0;
		m_currentAnimation = name;
        float newFrameTime = m_animations[name].frameTime;
        if (m_timer >= newFrameTime)
            m_timer = 0;
		m_end = false;

        Sprite* sprite = m_owner->GetComponent<Sprite>();
        int frameIndex = m_animations[name].frames[m_currentFrame];
        int columns = sprite->GetWidth() / m_width;
        int x = frameIndex % columns;
        int y = frameIndex / columns;
        sprite->SetSrcRect(x * m_width, y * m_height, m_width, m_height);
	}
}

bool Animation::IsAnimationEnded(){
	return m_end;
}

std::string Animation::AnimationIs(){
	return m_currentAnimation;
}

void Animation::Inspect()
{
    ImGui::Text("Current Animation: %s", m_currentAnimation.c_str());
    ImGui::Text("Current Frame: %d", m_currentFrame);
	ImGui::Text("Timer: %.2f", m_timer);
}


