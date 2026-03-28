#include "HypeBar.h"
#include "GameManager.h"
#include "imgui.h"

#include "TransformComponent.h"
#include "Entity.h"
#include "Sprite.h"
#include "SoundEffect.h"

HypeBar::HypeBar(float width, float height){
    

}

void HypeBar::SetHypeIndicator(float value) {
    TransformComponent* transform = m_owner->GetComponent<TransformComponent>();
    std::vector<Sprite*> sprites = m_owner->GetAllComponents<Sprite>();

    int widthBar = sprites[0]->GetWidth() * transform->GetScale().x;
    int offset = ((widthBar * value) - widthBar/2) * 0.96;
    sprites[1]->SetOffset({ (float)offset , 0.f });
}

void HypeBar::AddHype(float add) {
    m_hype += add;
    if (m_hype > 100) {
        m_hype = 100;
    }
    else if (m_hype < 0) {
        m_hype = 0;
    }
    SetHypeIndicator(m_hype / 100);
}

void HypeBar::Draw() {

}

void HypeBar::Inspect()
{
    if (ImGui::SliderFloat("Hype", &m_hype, 0, 100))
    {
        SetHypeIndicator(m_hype / 100);
    }
}