#include "RhythmSpawner.h"

#include "Scene.h"
#include "Entity.h"
#include "ArrowLogic.h"
#include "TransformComponent.h"
#include "TagComponent.h"
#include "BoxCollider.h"
#include "Music.h"

#include "../../include/json.hpp"

#include <fstream>
#include <iostream>
#include <algorithm>

#include "Sprite.h"

#include "RenderLayerComponent.h"

#include "SceneManager.h"
#include "PlayerMovement.h"

#include "EndMenu.h"
#include "HypeBar.h"
#include "SoundEffect.h"
#include "Button.h"

using json = nlohmann::json;

struct Lane
{
    const char* tag;
    float colliderW;
    float colliderH;
};

static const Lane LANE_DESCRIPTIONS[4] =
{
    { "LeftArrows",  50.f, 50.f },  
    { "RightArrows", 50.f, 50.f },   
    { "UpArrows",    50.f, 50.f },   
    { "DownArrows",  50.f, 50.f },   
};


static constexpr float SPAWN_X = 4600.f;
static const float LANE_Y[4] = { 5306.f, 5456.f, 5381.f, 5531.f };


RhythmSpawner::RhythmSpawner(Scene* scene, float travelTime) : m_scene(scene), m_travelTime(travelTime){
  
}


void RhythmSpawner::LoadMap(const std::string& jsonPath)
{
    m_events.clear();

    std::ifstream file(jsonPath);
    if (file.is_open() == false)
    {
        std::cout << "[RhythmSpawner] Cannot open: " << jsonPath << std::endl;
        return;
    }

    json j;
    file >> j;

    if (j.contains("notes") == false)
    {
        std::cout << "[RhythmSpawner] No 'notes' array in: " << jsonPath << std::endl;
        return;
    }

    for (const auto& n : j["notes"])
    {
        const float hitTime = n["time"].get<float>();
        const int   lane = n["lane"].get<int>();

        const float spawnTime = hitTime - m_travelTime;
        if (spawnTime < 0.f)
        {
            std::cout << "[RhythmSpawner] Note at t=" << hitTime << " ignoree\n";
            continue;
        }

        m_events.push_back({ spawnTime, hitTime, lane, false });
    }

    std::sort(m_events.begin(), m_events.end(),[](const NoteEvent& a, const NoteEvent& b) 
        { 
            return a.spawnTime < b.spawnTime; 
        });
}


void RhythmSpawner::Play()
{
    m_elapsed = 0.f;
    m_isPlaying = true;

    for (auto& e : m_events)
        e.spawned = false;

    if (Music* music = GetMusic())
        music->Play();
}


void RhythmSpawner::Pause()
{
    if (!m_isPlaying)
        return;

    m_isPlaying = false;

    if (Music* music = GetMusic())
        music->Pause();
}


void RhythmSpawner::Stop()
{
    m_isPlaying = false;
    m_elapsed = 0.f;

    if (Music* music = GetMusic())
        music->Stop();
}


void RhythmSpawner::Update(float dt)
{
    if (!m_isPlaying)
        return;

    m_elapsed += dt;

    for (auto& event : m_events)
    {
        if (event.spawned == false && m_elapsed >= event.spawnTime)
        {
            SpawnArrow(event.lane);
            event.spawned = true;
        }
    }

    const bool allSpawned = std::all_of(m_events.begin(), m_events.end(),[](const NoteEvent& e) 
        { 
            return e.spawned; 
        });

    if (allSpawned && m_events.empty() == true)
        m_isPlaying = false;

    if (IsLevelEnd() && SceneManager::GetInstance().GetCurrentScene()->FindEntityWithTag("Player")->GetComponent<PlayerMovement>()->IsInBossRoom()) {
        if (SceneManager::GetInstance().GetCurrentScene()->FindEntityWithTag("hypeBar")->GetComponent<HypeBar>()->GetHype() > 60.f) {
            
            SceneManager::GetInstance().GetCurrentScene()->FindEntityWithTag("PlayerBoss")->GetComponent<Sprite>()->SetTexture("../../../Assets/Boss/main_win.png");
            SceneManager::GetInstance().GetCurrentScene()->FindEntityWithTag("PlayerBoss")->GetComponent<Sprite>()->SetSrcRect(0, 0, 400, 410);
            SceneManager::GetInstance().GetCurrentScene()->FindEntityWithTag("Boss")->GetComponent<Sprite>()->SetTexture("../../../Assets/Boss/punk_loose.png");
            SceneManager::GetInstance().GetCurrentScene()->FindEntityWithTag("Boss")->GetComponent<Sprite>()->SetSrcRect(0, 0, 1293, 795);
            SceneManager::GetInstance().GetCurrentScene()->FindEntityWithTag("hypeBar")->GetComponent<SoundEffect>()->PlaySoundEffect();

            SceneManager::GetInstance().GetCurrentScene()->FindEntityWithTag("ButtonExit")->GetComponent<Sprite>()->SetVisible(true);
            SceneManager::GetInstance().GetCurrentScene()->FindEntityWithTag("ButtonExit")->GetComponent<Button>()->SetActivate(true);
        }
        else {

            SceneManager::GetInstance().GetCurrentScene()->FindEntityWithTag("PlayerBoss")->GetComponent<Sprite>()->SetTexture("../../../Assets/Boss/main_loose.png");
            SceneManager::GetInstance().GetCurrentScene()->FindEntityWithTag("PlayerBoss")->GetComponent<Sprite>()->SetSrcRect(0, 0, 400, 410);

            SceneManager::GetInstance().GetCurrentScene()->FindEntityWithTag("Boss")->GetComponent<Sprite>()->SetTexture("../../../Assets/Boss/punk_win.png");
            SceneManager::GetInstance().GetCurrentScene()->FindEntityWithTag("Boss")->GetComponent<Sprite>()->SetSrcRect(0, 0, 1293, 795);


            Entity* entity = SceneManager::GetInstance().GetCurrentScene()->FindEntityWithTag("ButtonExit");
            entity->GetComponent<RenderLayerComponent>()->SetLayer(12000);
            entity->GetComponent<Sprite>()->SetVisible(true);
            entity->GetComponent<Button>()->SetActivate(true);
            
        }
    }
}


Entity* RhythmSpawner::SpawnArrow(int lane)
{
    const Lane& desc = LANE_DESCRIPTIONS[lane];

    Entity* arrow = m_scene->CreateEntityNow();
    arrow->GetComponent<TransformComponent>()->SetPosition(SPAWN_X, LANE_Y[lane]);
    arrow->AddComponent<BoxCollider>(desc.colliderW, desc.colliderH)->SetTrigger(true);
    arrow->AddComponent<ArrowLogic>();
    arrow->GetComponent<TagComponent>()->AddTag(desc.tag);
    arrow->GetComponent<TagComponent>()->AddTag("Arrow");
    arrow->GetComponent<RenderLayerComponent>()->SetLayer(150);

    if (lane == 0) {
        Sprite* sprite = arrow->AddComponent<Sprite>("../../../Assets/ux/gauche.png");
        arrow->GetComponent<TransformComponent>()->SetScale(0.5, 0.5);
        arrow->GetComponent<RenderLayerComponent>()->SetLayer(110);
        

    }
    else if (lane == 1) {
        Sprite* sprite = arrow->AddComponent<Sprite>("../../../Assets/ux/droit.png");
        arrow->GetComponent<TransformComponent>()->SetScale(0.5, 0.5);
        arrow->GetComponent<RenderLayerComponent>()->SetLayer(110);

        //sprite->SetUI(true);

    }
    else if (lane == 2) {
        Sprite* sprite = arrow->AddComponent<Sprite>("../../../Assets/ux/haut.png");
        arrow->GetComponent<TransformComponent>()->SetScale(0.5, 0.5);
        arrow->GetComponent<RenderLayerComponent>()->SetLayer(110);

        //sprite->SetUI(true);

    }
    else if (lane == 3) {
        Sprite* sprite = arrow->AddComponent<Sprite>("../../../Assets/ux/bas.png");
        arrow->GetComponent<TransformComponent>()->SetScale(0.5, 0.5);
        arrow->GetComponent<RenderLayerComponent>()->SetLayer(110);

        //sprite->SetUI(true);

    }

    

    return arrow;
}


Music* RhythmSpawner::GetMusic()
{
    Entity* musicEnt = m_scene->FindEntityWithTag("rhythmMusic");

    if(musicEnt == nullptr)
        return nullptr;

    return musicEnt->GetComponent<Music>();
}


bool RhythmSpawner::IsLevelEnd(){
    for (auto& n : m_events) {
        if (n.spawned == false) {
            return false;
        }
    }
    if (SceneManager::GetInstance().GetCurrentScene()->FindAllEntitiesWithTag("Arrow").size() > 0) {
        return false;
    }
    return true;
}
