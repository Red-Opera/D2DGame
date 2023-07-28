// Scene.cpp

#include "stdafx.h"
#include "Scene.h"
#include "Scene/Actor.h"

#include "Core/SubSystem/Graphics.h"

#include "Scene/Component/CameraComponent.h"
#include "Scene/Component/MeshRendererComponent.h"

#include "Scene/Component/MoveComponent/TransformComponent.h"
#include "Scene/Component/MoveComponent/MoveScriptComponent.h"
#include "Scene/Component/MoveComponent/AIScriptComponent.h"
#include "Scene/Component/MoveComponent/TraceComponent.h"
#include "Scene/Component/MoveComponent/CollisionComponent.h"

#include "Scene/Component/AnimationComponent/AnimatorComponent.h"
#include "Scene/Component/AnimationComponent/BackGroundManager.h"
#include "Scene/Component/AnimationComponent/GaugeBarComponent.h"

#include "Scene/Component/AttackComponent/MonsterAttackComponent.h"
#include "Scene/Component/AttackComponent/BulletComponent.h" 
#include "Scene/Component/AttackComponent/PlayerAttackComponent.h"
#include "Scene/Component/AttackComponent/PlayerSkillComponent.h"

#define WINDOWTODIRECTXPOS(pos, defaultSize, toSize, spriteSize) ((2 * pos / defaultSize - 1) * (toSize / 2) + spriteSize)

Scene::Scene(class Context* const context) : context(context)
{
	renderer = context->GetSubSystem<Renderer>();

	// ===============================================================
	//							  ��� ��ü
	// ===============================================================

	// ī�޶�
	std::shared_ptr<Actor> camera = CreateActor();
	camera->AddComponent<CameraComponent>();
	camera->AddComponent<TraceComponent>();
	camera->SetName("MainCamera");
	this->camera = camera;

	// �� ���
	std::shared_ptr<Actor> background = CreateActor();
	background->AddComponent<MeshRendererComponent>();
	background->SetName("BackGround");

	auto backgroundTexture = background->AddComponent<BackGroundComponent>();	// ��� Component �Է�
	background->GetComponent<BackGroundComponent>()->SetMainCamera(camera);		// ����� ���� ���� ī�޶� ����

	backgroundTexture->SetTexture("Assets/Texture/Cloud.png");					// ��� �̹��� ����
	backgroundTexture->GetTexture()->FullSize();								// ��� ũ�� ����
}

Scene::~Scene()
{
	
}

void Scene::Update()
{
	// ��� ��ü ������Ʈ
	for (auto actor = actors.begin(); actor != actors.end();)
	{
		(*actor)->Update();

		while ((*actor)->GetName() == "")
		{
			actor = actors.erase(actor);
			if (actor == actors.end()) return;
		}

		//ImGuiControl::OutputTransform(*actor, "Trasnform", FigureType::Input3, FigureType::Slider3, 0.0f, 100.0f);

		actor++;
	}

	// ������
	if (is_update)
	{
		renderer->UpdateRenderables(this);
		is_update = false;
	}
}

const std::shared_ptr<class Actor> Scene::CreateActor(const bool& is_active)
{
	is_update = true;
	std::shared_ptr<class Actor> actor = std::make_shared<Actor>(context);

	actor->SetActive(is_active);
	AddActor(actor);

	actor->SetScene(this); // ��� ��ü�� actor�� ���� Scene�� ���

	return actor;
}

void Scene::AddActor(const std::shared_ptr<class Actor>& actor)
{
	actors.emplace_back(actor);
}

void Scene::EreaseActor(const std::shared_ptr<class Actor>& actor)
{
	if (std::find(actors.begin(), actors.end(), actor) == actors.end())
	{
		assert(false && "ERROR (Scene1) : �ش� �ϴ� actor�� �������� ����");
		return;
	}

	actors.erase(std::find(actors.begin(), actors.end(), actor));
	actor->~Actor();
}