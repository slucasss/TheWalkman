#include "RythmeText.h"
#include "GameManager.h"

void RythmeText::Update(){
	m_lifeTimer += GameManager::Get()->GetDeltaTime();

	if (m_lifeTimer >= 2.f) {
		m_owner->Destroy();
	}
}
