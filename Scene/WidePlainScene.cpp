// WidePlainScene.cpp

#include "stdafx.h"
#include "WidePlainScene.h"
#include "Scene/Actor.h"

#include "Core/SubSystem/Graphics.h"

#include "Scene/Component/CameraComponent.h"
#include "Scene/Component/MeshRendererComponent.h"

#include "Scene/Component/MoveComponent/TransformComponent.h"
#include "Scene/Component/MoveComponent/MoveScriptComponent.h"
#include "Scene/Component/MoveComponent/AIScriptComponent.h"
#include "Scene/Component/MoveComponent/TraceComponent.h"
#include "Scene/Component/MoveComponent/CollisionComponent.h"
#include "Scene/Component/MoveComponent/TeleportComponent.h"

#include "Scene/Component/AnimationComponent/AnimatorComponent.h"
#include "Scene/Component/AnimationComponent/BackGroundManager.h"
#include "Scene/Component/AnimationComponent/GaugeBarComponent.h"

#include "Scene/Component/AttackComponent/MonsterAttackComponent.h"
#include "Scene/Component/AttackComponent/BulletComponent.h" 
#include "Scene/Component/AttackComponent/PlayerAttackComponent.h"
#include "Scene/Component/AttackComponent/PlayerSkillComponent.h"

#define WINDOWTODIRECTXPOS(pos, defaultSize, toSize, spriteSize) ((2 * pos / defaultSize - 1) * (toSize / 2) + spriteSize)

WidePlainScene::WidePlainScene(class Context* const context) : Scene(context)
{
	// 앞 배경
	std::shared_ptr<Actor> frontground = CreateActor();
	frontground->AddComponent<MeshRendererComponent>();
	frontground->SetName("FrontGround");

	auto frontgroundTexture = frontground->AddComponent<BackGroundComponent>();

	frontgroundTexture->SetTexture("Assets/Texture/Grass.png");
	frontgroundTexture->GetTexture()->FullSize(false, 960.0f, 480.0f);

	frontground->GetComponent<TransformComponent>()->SetScale(D3DXVECTOR3(WorldX, WorldY, 1.0f));
	frontground->GetComponent<TransformComponent>()->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// 플레이어
	std::shared_ptr<Actor> player = CreateActor();
	player->AddComponent<MeshRendererComponent>();
	player->AddComponent<CollisionComponent>();
	player->AddComponent<MoveScriptComponent>();

	player->GetComponent<MoveScriptComponent>()->SetMoveSpeed(4.0f);

	auto animator = player->AddComponent<AnimatorComponent>();
	animator->SetAnimationFrameName(4, "FrontIdle", "LeftIdle", "RightIdle", "BackIdle");
	animator->AddAnimation("Assets/Animation/Player/PlayerIdle.xml");
	animator->SetAnimationFrameName(4, "FrontMove", "LeftMove", "RightMove", "BackMove");
	animator->AddAnimation("Assets/Animation/Player/PlayerMove.xml");

	animator->SetCurrentAnimation("BackIdle");
	animator->SetAnimationMode(AnimationMode::Play);

	player->GetComponent<TransformComponent>()->SetScale(D3DXVECTOR3(70.0f, 100.0f, 1.0f));
	player->GetComponent<TransformComponent>()->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	player->SetName("Player");

	// 슬라임
	std::list<std::shared_ptr<Actor>> monster;

	for (int i = 0; i < 1; i++)
	{
		monster.emplace_back(CreateActor());
		monster.back()->AddComponent<MeshRendererComponent>();
		monster.back()->AddComponent<AIScriptComponent>();
		monster.back()->AddComponent<TraceComponent>();
		monster.back()->AddComponent<MonsterAttackComponent>();
		monster.back()->AddComponent<CollisionComponent>();
		monster.back()->AddComponent<BulletComponent>();

		auto monsterAnimator = monster.back()->AddComponent<AnimatorComponent>();
		monsterAnimator->SetAnimationFrameName(2, "LeftMove", "RightMove");
		monsterAnimator->AddAnimation("Assets/Animation/Monster/GreenMonsterMove.xml");
		monsterAnimator->SetAnimationFrameName(2, "LeftAttack", "RightAttack");
		monsterAnimator->AddAnimation("Assets/Animation/Monster/GreenMonsterAttack.xml");
		monsterAnimator->SetAnimationFrameName(2, "LeftDeath", "RightDeath");
		monsterAnimator->AddAnimation("Assets/Animation/Monster/GreenMonsterDeath.xml");

		monsterAnimator->SetCurrentAnimation("RightMove");
		monsterAnimator->SetAnimationMode(AnimationMode::Play);

		monster.back()->GetComponent<TransformComponent>()->SetScale(D3DXVECTOR3(50.0f, 50.0f, 1.0f));
		monster.back()->GetComponent<TransformComponent>()->SetPosition(D3DXVECTOR3(-45.0f, 300.0f, 1.0f));

		monster.back()->GetComponent<TraceComponent>()->SetSpeed(15.0f);
		monster.back()->GetComponent<TraceComponent>()->SetTarget(player);
		monster.back()->SetName(std::to_string(i) + "GreenSlimeMonster");

		monster.back()->GetComponent<MonsterAttackComponent>()->SetDamage(15.0f);
		monster.back()->GetComponent<MonsterAttackComponent>()->SetAttackRange(200.0f);
		monster.back()->GetComponent<MonsterAttackComponent>()->SetAttackDelay(0.75f);

		monster.back()->GetComponent<BulletComponent>()->SetStyleSpeed(BulletStyle::Long, 5.0f);
		monster.back()->GetComponent<BulletComponent>()->SetSprite("GreenMonsterBullet", "Assets/Animation/Bullet/GreenMonsterBullet.xml");

		monster.back()->SetActive(false);
	}

	// Goblin
	std::list<std::shared_ptr<Actor>> monster2;

	for (int i = 0; i < 1; i++)
	{
		monster2.emplace_back(CreateActor());

		monster2.back()->AddComponent<MeshRendererComponent>();
		monster2.back()->AddComponent<AIScriptComponent>();
		monster2.back()->AddComponent<TraceComponent>();
		monster2.back()->AddComponent<MonsterAttackComponent>();
		monster2.back()->AddComponent<BulletComponent>();

		auto monster2Animator = monster2.back()->AddComponent<AnimatorComponent>();
		monster2Animator->SetAnimationFrameName(2, "LeftMove", "RightMove");
		monster2Animator->AddAnimation("Assets/Animation/Monster/GoblinMonsterMove.xml");
		monster2Animator->SetAnimationFrameName(2, "LeftAttack", "RightAttack");
		monster2Animator->AddAnimation("Assets/Animation/Monster/GoblinMonsterAttack.xml");
		monster2Animator->SetAnimationFrameName(2, "LeftDeath", "RightDeath");
		monster2Animator->AddAnimation("Assets/Animation/Monster/GoblinMonsterDeath.xml");

		monster2Animator->SetCurrentAnimation("RightMove");
		monster2Animator->SetAnimationMode(AnimationMode::Play);

		monster2.back()->GetComponent<TransformComponent>()->SetScale(D3DXVECTOR3(120.0f, 100.0f, 1.0f));
		monster2.back()->GetComponent<TransformComponent>()->SetPosition(D3DXVECTOR3(-1084.0f, 0.0f, 1.0f));

		monster2.back()->GetComponent<TraceComponent>()->SetSpeed(25.0f);
		monster2.back()->GetComponent<TraceComponent>()->SetTarget(player);
		monster2.back()->SetName(std::to_string(i) + std::string("GoblinMonster"));

		monster2.back()->GetComponent<MonsterAttackComponent>()->SetDamage(20.0f);
		monster2.back()->GetComponent<MonsterAttackComponent>()->SetAttackRange(125.0f);	// 100.0f -> 145.0f : (플레이어 사이즈 + 몬스터 사이즈) / 2
		monster2.back()->GetComponent<MonsterAttackComponent>()->SetAttackDelay(1.05f);
		monster2.back()->GetComponent<MonsterAttackComponent>()->SetSizeChange(1.45f, ResizeTo::X);

		monster2.back()->GetComponent<BulletComponent>()->SetStyleSpeed(BulletStyle::Short, 1.0f);
		monster2.back()->SetActive(false);
	}

	// 플레이어 체력 바
	std::shared_ptr<Actor> gaugeBar = CreateActor();
	gaugeBar->AddComponent<TransformComponent>();
	gaugeBar->AddComponent<GaugeBarComponent>();

	BarTransform barTransform = { D3DXVECTOR2(0.0f, 100.0f), D3DXVECTOR2(0.0f, 75.0f), D3DXVECTOR2(125.0f, 13.0f), D3DXVECTOR2(125.0f, 13.0f) };
	gaugeBar->GetComponent<GaugeBarComponent>()->SetBasicState(2000.0f, 200.0f);
	gaugeBar->GetComponent<GaugeBarComponent>()->SetBarTransform(barTransform);

	gaugeBar->GetComponent<TransformComponent>()->SetParent(player->GetComponent<TransformComponent>());	// 부모 등록
	player->GetComponent<TransformComponent>()->AddChild(player->GetComponent<TransformComponent>());		// 자식 등록

	gaugeBar->GetComponent<TransformComponent>()->SetScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	gaugeBar->SetName("GaugeBar");

	player->AddComponent<PlayerAttackComponent>();
	player->GetComponent<PlayerAttackComponent>()->SetAttackDelay(0.35f);
	player->GetComponent<PlayerAttackComponent>()->SetAttackRange(50.0f);
	player->GetComponent<PlayerAttackComponent>()->SetDamage(2000.0f);
	player->GetComponent<PlayerAttackComponent>()->SetStyleSpeed(PlayerBulletStyle::Short);

	player->AddComponent<PlayerSkillComponent>();
	player->GetComponent<PlayerSkillComponent>()->SetManaComsum(8);

	for (const auto& object : monster)
	{
		player->GetComponent<CollisionComponent>()->SetCollistionTarget(object);
		player->GetComponent<CollisionComponent>()->SetCollisionArea(StartArea::Bottom, 1.0f);
	}
	
	// 텔레포트
	std::shared_ptr<Actor> nextMap = CreateActor();
	nextMap->AddComponent<TransformComponent>();
	nextMap->GetComponent<TransformComponent>()->SetPosition(D3DXVECTOR3(1150, -80.0f, 1.0f));
	nextMap->GetComponent<TransformComponent>()->SetScale(D3DXVECTOR3(70.0f, 100.0f, 1.0f));
	nextMap->SetName("TeleportArea");

	auto frontgroundTexture2 = nextMap->AddComponent<BackGroundComponent>();

	frontgroundTexture2->SetTexture("Assets/Texture/null.png");
	frontgroundTexture2->GetTexture()->FullSize(false, 70.0f, 100.0f);

	nextMap->GetComponent<TransformComponent>()->SetScale(D3DXVECTOR3(70.0f, 100.0f, 1.0f));

	nextMap->AddComponent<TeleportComponent>();
	nextMap->GetComponent<TeleportComponent>()->SetNextMap("MainScene");

	camera->GetComponent<TraceComponent>()->SetTarget(player);

	std::queue<int> key; key.push(DIK_1); key.push(DIK_2);
	std::queue<PlayerBulletStyle> bulletStyle; bulletStyle.emplace(PlayerBulletStyle::Short); bulletStyle.emplace(PlayerBulletStyle::Long);
	std::queue<float> bulletSpeed; bulletSpeed.emplace(40.0f);

	ChangeAttackTypeManager::ChangeType(player, key, bulletStyle, bulletSpeed);
}

WidePlainScene::~WidePlainScene()
{
	actors.clear();
}

void WidePlainScene::Update()
{
	BarTransform barTransform = { D3DXVECTOR2(0.0f, 100.0f), D3DXVECTOR2(0.0f, 75.0f), D3DXVECTOR2(125.0f, 13.0f), D3DXVECTOR2(125.0f, 13.0f) };
	static int count = 1;

	for (const auto& monster : actors)
	{
		if (monster->GetName().size() < 18) continue;

		if ((monster->GetName().substr(1) == "GreenSlimeMonster") && !monster->IsActive())
		{
			monster->SetActive(true);

			// 초록 슬라임 체력 바
			std::shared_ptr<Actor> gaugeBar2 = CreateActor();
			gaugeBar2->AddComponent<TransformComponent>();
			gaugeBar2->AddComponent<GaugeBarComponent>();

			gaugeBar2->GetComponent<GaugeBarComponent>()->SetBasicState(250.0f, 25.0f);
			gaugeBar2->GetComponent<GaugeBarComponent>()->SetBarTransform(barTransform);

			gaugeBar2->GetComponent<TransformComponent>()->SetParent(monster->GetComponent<TransformComponent>());	// 부모 등록
			monster->GetComponent<TransformComponent>()->AddChild(gaugeBar2->GetComponent<TransformComponent>());	// 자식 등록

			gaugeBar2->GetComponent<TransformComponent>()->SetScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
			gaugeBar2->SetName("GaugeBar2" + std::to_string(count));

			break;
		}
	}

	for (const auto& monster : actors)
	{
		if (monster->GetName().size() < 14) continue;

		if ((monster->GetName().substr(1) == "GoblinMonster") && !monster->IsActive())
		{
			monster->SetActive(true);

			// 고블린 체력 바
			std::list<std::shared_ptr<Actor>> gaugeBar3;

			gaugeBar3.emplace_back(CreateActor());
			gaugeBar3.back()->AddComponent<TransformComponent>();
			gaugeBar3.back()->AddComponent<GaugeBarComponent>();

			gaugeBar3.back()->GetComponent<GaugeBarComponent>()->SetBasicState(100.0f, 50.0f);
			gaugeBar3.back()->GetComponent<GaugeBarComponent>()->SetBarTransform(barTransform);

			gaugeBar3.back()->GetComponent<TransformComponent>()->SetParent(monster->GetComponent<TransformComponent>());	// 부모 등록
			monster->GetComponent<TransformComponent>()->AddChild(gaugeBar3.back()->GetComponent<TransformComponent>());		// 자식 등록

			gaugeBar3.back()->GetComponent<TransformComponent>()->SetScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
			gaugeBar3.back()->SetName("GaugeBar3" + std::to_string(count));

			count++;

			break;
		}
	}

	Scene::Update();
}