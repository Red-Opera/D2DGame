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
	//							  배경 객체
	// ===============================================================

	// 카메라
	std::shared_ptr<Actor> camera = CreateActor();
	camera->AddComponent<CameraComponent>();
	camera->AddComponent<TraceComponent>();
	camera->SetName("MainCamera");
	this->camera = camera;

	// 뒷 배경
	std::shared_ptr<Actor> background = CreateActor();
	background->AddComponent<MeshRendererComponent>();
	background->SetName("BackGround");

	auto backgroundTexture = background->AddComponent<BackGroundComponent>();	// 배경 Component 입력
	background->GetComponent<BackGroundComponent>()->SetMainCamera(camera);		// 배경이 따라갈 메인 카메라 설정

	backgroundTexture->SetTexture("Assets/Texture/Cloud.png");					// 배경 이미지 설정
	backgroundTexture->GetTexture()->FullSize();								// 배경 크기 설정
}

Scene::~Scene()
{
	
}

void Scene::Update()
{
	// 모든 객체 업데이트
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

	// 랜더링
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

	actor->SetScene(this); // 모든 객체의 actor에 현재 Scene을 등록

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
		assert(false && "ERROR (Scene1) : 해당 하는 actor가 존재하지 않음");
		return;
	}

	actors.erase(std::find(actors.begin(), actors.end(), actor));
	actor->~Actor();
}