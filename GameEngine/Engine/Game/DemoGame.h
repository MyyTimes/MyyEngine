#pragma once
#include <iostream>

#include "../ECS/GameObjectManager.h"

#include "../Scene/Scene.h"

#include "../Systems/ParticleSystem.h"
#include "../Systems/PhysicsSystem.h"
#include "../Systems/RenderSystem.h"
#include "../Systems/AnimationSystem.h"

#include "../Assets/SoundManager.h"
#include "../Assets/AssetManager.h"

#include "../Core/Logger.h"
#include "../Debug/DebugRenderer.h"

class DemoGame : public Scene
{
public:
    DemoGame(SDL_Renderer*, RenderQueue*, AssetManager*, SoundManager*);
    ~DemoGame() override = default;

    void OnEnter() override;
    void OnExit() override;
    void OnUpdate(float) override;
    void OnRender() override;

private:
    SDL_Renderer* m_renderer;
    RenderQueue* m_RQ;
    AssetManager* m_assets;
	SoundManager* m_sounds;

	// Camera
	std::unique_ptr<Camera2D> m_camera;
	// Debug Renderer
	std::unique_ptr<DebugRenderer> m_debugRenderer;

	GameObjectManager m_gameObjectManager;

    // Systems
    std::vector<std::unique_ptr<ISystem>> m_systems;

    // Animation Clips
    AnimationClip idleAnim;

    // Objects
    GameObject* m_player = nullptr;
};
