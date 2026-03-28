#pragma once

#include <string>
#include <vector>
#include <functional>

class Scene;
class Entity;
class Music;

class RhythmSpawner
{
public:
    RhythmSpawner(Scene* scene, float travelTime = 6.f);
    void LoadMap(const std::string& jsonPath);
    void Play();
    void Pause();
    void Stop();

    void Update(float dt);

    bool IsPlaying() const { return m_isPlaying; }

private:

    struct NoteEvent
    {
        float spawnTime;    
        float hitTime;     
        int lane;         
        bool spawned = false;
    };

    Entity* SpawnArrow(int lane);

    Music* GetMusic();

    Scene* m_scene = nullptr;
    float m_travelTime = 6.f;
    float m_elapsed = 0.f;
    bool m_isPlaying = false;

    std::vector<NoteEvent> m_events;

    bool IsLevelEnd();

};