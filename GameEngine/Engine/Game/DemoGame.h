#pragma once
#include <iostream>

#include "../ECS/GameObjectManager.h"

#include "../Scene/Scene.h"
#include "../Assets/AssetManager.h"

#include "../Systems/PhysicsSystem.h"
#include "../Systems/RenderSystem.h"
#include "../Systems/AnimationSystem.h"

class DemoGame : public Scene
{
public:
    DemoGame(SDL_Renderer* renderer, RenderQueue* rq, AssetManager* assets);
    ~DemoGame() override = default;

    void OnEnter() override;
    void OnExit() override;
    void OnUpdate(float) override;
    void OnRender() override;

private:
    SDL_Renderer* m_renderer;
    RenderQueue* m_RQ;
    AssetManager* m_assets;

    GameObjectManager m_gameObjectManager;

    // Systems
    std::vector<std::unique_ptr<ISystem>> m_systems;

    // Animation Clips
    AnimationClip idleAnim;

    // Objects
    GameObject* m_player = nullptr;
};