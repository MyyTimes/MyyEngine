#include "DemoGame.h"

DemoGame::DemoGame(SDL_Renderer* r, RenderQueue* rq, AssetManager* a) : m_renderer{ r }, m_RQ{ rq }, m_assets{ a } {}

void DemoGame::OnEnter()
{
	// ---------- SYSTEMS AND POINTERS ----------
	// Physics system
	auto physSys = std::make_unique<PhysicsSystem>(&m_gameObjectManager);
	if (!physSys->Init())
	{
		std::cerr << "Physics System Init Failed!" << std::endl;
		return;
	}
	// Render System
	auto renderSys = std::make_unique<RenderSystem>(&m_gameObjectManager, m_RQ, m_renderer);
	if (!renderSys->Init())
	{
		std::cerr << "Render System Init Failed!" << std::endl;
		return;
	}
	// Animation System
	auto animSys = std::make_unique<AnimationSystem>(&m_gameObjectManager);
	if (!animSys->Init())
	{
		std::cerr << "Animation System Init Failed!" << std::endl;
		return;
	}

	// Borders
	float m_windowWidth = 800.f;
	float m_windowHeight = 800.f;
	physSys->CreateBox(m_windowWidth / 2, m_windowHeight - 5, m_windowWidth, 10, false, 1.0f, 0.5f);
	physSys->CreateBox(m_windowWidth / 2, 5, m_windowWidth, 10, false, 1.0f, 0.5f);
	physSys->CreateBox(5, m_windowHeight / 2, 10, m_windowHeight, false, 1.0f, 0.5f);
	physSys->CreateBox(m_windowWidth - 5, m_windowHeight / 2, 10, m_windowHeight, false, 1.0f, 0.5f);

	// PLAYER
	float startX = 100.0f;
	float startY = 200.0f;
	m_player = m_gameObjectManager.CreateGameObject();
	m_player->AddComponent<TransformComponent>();
	m_player->GetComponent<TransformComponent>()->position = Vector2(startX, startY);
	m_player->AddComponent<SpriteComponent>();
	m_player->GetComponent<SpriteComponent>()->texture = m_assets->GetTexture("brick", "Assets/Textures/bricks.png");
	// its physics
	auto* playerPhy = m_player->AddComponent<RigidbodyComponent>();
	playerPhy->isStatic = false;
	playerPhy->friction = 0.3f;
	playerPhy->density = 0.8f;
	playerPhy->body = physSys->CreateBox(startX, startY, 32.f, 32.f, !playerPhy->isStatic, playerPhy->density, playerPhy->friction);
	// its animation
	auto* playerAnim = m_player->AddComponent<AnimationComponent>();

	idleAnim.name = "IDLE";
	idleAnim.loop = true;
	idleAnim.fps = 12.0f;
	idleAnim.texture = m_assets->GetTexture("player", "Assets/Textures/player.png");
	//idleAnim.frames.push_back(SDL_Rect{ 0,   0, 32, 32 }); // 1 Frame
	//idleAnim.frames.push_back(SDL_Rect{ 32,  0, 32, 32 }); // 2 Frame
	//idleAnim.frames.push_back(SDL_Rect{ 64,  0, 32, 32 }); // 3 Frame

	for (int i = 0; i < 3; i++) {
		idleAnim.frames.push_back(SDL_Rect{ i * 64, 0, 64, 64 });
	}

	playerAnim->Play(&idleAnim);

	// Add systems to vector
	m_systems.push_back(std::move(physSys));
	m_systems.push_back(std::move(renderSys));
	m_systems.push_back(std::move(animSys));

	m_player->Init();
}

void DemoGame::OnUpdate(float dt)
{
	if (m_player)
	{
		auto* rigidbody = m_player->GetComponent<RigidbodyComponent>();
		if (rigidbody)
		{
			if (m_input.IsKeyDown(SDL_SCANCODE_D)) rigidbody->ApplyForce(50.0f, 0.0f);
			if (m_input.IsKeyDown(SDL_SCANCODE_A)) rigidbody->ApplyForce(-50.0f, 0.0f);
			if (m_input.IsKeyDown(SDL_SCANCODE_W)) rigidbody->ApplyForce(0.0f, -50.0f);
			if (m_input.IsKeyDown(SDL_SCANCODE_S)) rigidbody->ApplyForce(0.0f, 50.0f);
		}
	}

	m_gameObjectManager.Update(dt);

	for (auto& sys : m_systems)
	{
		sys->Update(dt);
	}

	m_gameObjectManager.Refresh();
}

void DemoGame::OnRender()
{
	for (auto& sys : m_systems)
	{
		sys->Render();
	}
}

void DemoGame::OnExit()
{
	m_gameObjectManager.Clear();

	for (auto& sys : m_systems)
	{
		sys->Shutdown();
	}

	std::cout << "OnExit: DemoScene" << std::endl;
}
