#include "DemoGame.h"

DemoGame::DemoGame(SDL_Renderer* r, RenderQueue* rq, AssetManager* a, SoundManager* s) : m_renderer{ r }, m_RQ{ rq }, m_assets{ a }, m_sounds{ s } {}

void DemoGame::OnEnter()
{
	// ---------- SYSTEMS AND POINTERS ----------
	// Create Camera
	m_camera = std::make_unique<Camera2D>(800, 800);
	m_camera->SetZoom(1.1f);
	//m_camera->Shake(2.f, 3.f);
	// Physics system
	auto physSys = std::make_unique<PhysicsSystem>(&m_gameObjectManager, 0.f, 0.f);
	if (!physSys->Init())
	{
		std::cerr << "Physics System Init Failed!" << std::endl;
		return;
	}
	// Render System
	auto renderSys = std::make_unique<RenderSystem>(&m_gameObjectManager, m_RQ, m_camera.get());
	if (!renderSys->Init())
	{
		std::cerr << "Render System Init Failed!" << std::endl;
		return;
	}
	// > Debug Render
	m_debugRenderer = std::make_unique<DebugRenderer>(&m_gameObjectManager, m_camera.get(), m_renderer);
	// Animation System
	auto animSys = std::make_unique<AnimationSystem>(&m_gameObjectManager);
	if (!animSys->Init())
	{
		std::cerr << "Animation System Init Failed!" << std::endl;
		return;
	}
	// Particle System
	auto particleSys = std::make_unique<ParticleSystem>(m_camera.get(), m_renderer);
	if (!particleSys->Init())
	{
		std::cerr << "Particle System Init Failed!" << std::endl;
		return;
	}

	// PLAYER
	float startX = 100.0f;
	float startY = 200.0f;
	m_player = m_gameObjectManager.CreateGameObject();
	m_player->AddComponent<TransformComponent>()->position = Vector2(startX, startY);;
	//m_player->GetComponent<TransformComponent>()->position = Vector2(startX, startY);
	SpriteComponent *sprCom = m_player->AddComponent<SpriteComponent>();
	sprCom->texture = m_assets->GetTexture("brick", "Assets/Textures/bricks.png");
	sprCom->props.blendMode = BlendMode::Additive;
	//m_player->GetComponent<SpriteComponent>()->texture = m_assets->GetTexture("brick", "Assets/Textures/bricks.png");
	//m_player->GetComponent<SpriteComponent>()->props.blendMode = BlendMode::Additive;
	m_player->AddComponent<ParticleComponent>();

	// its physics
	auto* playerPhy = m_player->AddComponent<RigidbodyComponent>();
	playerPhy->isStatic = false;
	playerPhy->friction = 0.7f;
	playerPhy->density = 0.8f;
	playerPhy->body = physSys->CreateBox(startX, startY, 32.f, 32.f, !playerPhy->isStatic, playerPhy->density, playerPhy->friction);
	// its animation
	auto* playerAnim = m_player->AddComponent<AnimationComponent>();

	idleAnim.name = "IDLE";
	idleAnim.loop = true;
	idleAnim.fps = 12.0f;
	idleAnim.texture = m_assets->GetTexture("player", "Assets/Textures/player.png");
	for (int i = 0; i < 3; i++)
	{
		idleAnim.frames.push_back(SDL_Rect{ i * 64, 0, 64, 64 });
	}

	playerAnim->Play(&idleAnim);

	// Add systems to vector
	m_systems.push_back(std::move(physSys));
	m_systems.push_back(std::move(renderSys));
	m_systems.push_back(std::move(animSys));
	m_systems.push_back(std::move(particleSys));

	m_player->Init();
	m_camera->SetTarget(m_player);
}

void DemoGame::OnUpdate(float dt)
{
	if (m_player)
	{
		auto* rigidbody = m_player->GetComponent<RigidbodyComponent>();
		if (rigidbody)
		{
			if (m_input.IsKeyDown(SDL_SCANCODE_D)) rigidbody->ApplyForce(5.0f, 0.0f);
			if (m_input.IsKeyDown(SDL_SCANCODE_A)) rigidbody->ApplyForce(-5.0f, 0.0f);
			if (m_input.IsKeyDown(SDL_SCANCODE_W)) rigidbody->ApplyForce(0.0f, -5.0f);
			if (m_input.IsKeyDown(SDL_SCANCODE_S)) rigidbody->ApplyForce(0.0f, 5.0f);
		}
	}

	m_gameObjectManager.Update(dt);

	for (auto& sys : m_systems)
	{
		sys->Update(dt);
	}

	if (m_camera)
	{
		m_camera->Update(dt);
	}

	std::cout << m_player->GetComponent<TransformComponent>()->position.x << std::endl;

	m_gameObjectManager.Refresh();


	// PARTICLE
	if (m_input.IsKeyPressed(SDL_SCANCODE_SPACE))
	{
		ParticleProps magicDust;
		magicDust.position = { 400.0f, 300.0f };
		magicDust.velocity = { 0.0f, -30.0f };
		magicDust.velocityVariation = { 150.0f, 150.0f };
		magicDust.colorBegin = { 0, 255, 255, 255 };
		magicDust.colorEnd = { 0, 100, 255, 0 };   

		magicDust.sizeBegin = 8.0f;
		magicDust.sizeEnd = 0.0f;
		magicDust.sizeVariation = 3.0f; 

		magicDust.lifeTime = 1.2f;

		for (int i = 0; i < 40; i++)
		{
			ParticleSystem::PushParticleProps(magicDust);
		}
	}

	// Debug F1
	if (m_input.IsKeyPressed(SDL_SCANCODE_F1))
	{
		m_debugRenderer->Toggle();

		if (m_debugRenderer->IsEnabled())
			LOG_INFO("Debug Renderer is enable.");
		else
			LOG_INFO("Debug Renderer is disable.");
	}
}

void DemoGame::OnRender()
{
	Renderer2D::BeginScene();

	// ========== GAME & GAME OBJECTS ==========

	for (auto& sys : m_systems)
	{
		sys->Render();
	}

	// ========== DEBUG & UI ==========

	SpriteProperties props;
	props.blendMode = BlendMode::None;
	props.rotation = 45.0f;
	props.flip = SDL_FLIP_HORIZONTAL;
	props.colorTint = { 0, 255, 0, 255 };
	SDL_Texture* brickTexture = m_assets->GetTexture("brick");

	Vector2f worldBrickPos = { 200.0f, 200.0f };
	Vector2f screenBrickPos = m_camera->WorldToScreen(worldBrickPos);
	Renderer2D::DrawSprite(brickTexture, screenBrickPos, props);
	
	Renderer2D::DrawRect({ 100, 100 }, { 50, 50 }, { 255, 0, 0, 128 }, true);

	Vector2f worldCirclePos = { 400.0f, 400.0f };
	Vector2f screenCirclePos = m_camera->WorldToScreen(worldCirclePos);
	Renderer2D::DrawCircle(screenCirclePos, 150.0f * m_camera->GetZoom(), { 0, 0, 255, 255 }, false);

	if (m_debugRenderer)
	{
		m_debugRenderer->Render();
	}

	// end batch
	Renderer2D::EndScene();
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
