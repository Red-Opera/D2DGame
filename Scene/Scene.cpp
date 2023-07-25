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
	timer = context->GetSubSystem<Timer>();

	// ===============================================================
	//							  배경 객체
	// ===============================================================

	// 카메라
	std::shared_ptr<Actor> camera = CreateActor();
	camera->AddComponent<CameraComponent>();
	camera->AddComponent<TraceComponent>();
	camera->SetName("MainCamera");

	// 뒷 배경
	std::shared_ptr<Actor> background = CreateActor();
	background->AddComponent<MeshRendererComponent>();
	background->SetName("BackGround");

	auto backgroundTexture = background->AddComponent<BackGroundComponent>();	// 배경 Component 입력
	background->GetComponent<BackGroundComponent>()->SetMainCamera(camera);		// 배경이 따라갈 메인 카메라 설정
	
	backgroundTexture->SetTexture("Assets/Texture/Cloud.png");					// 배경 이미지 설정
	backgroundTexture->GetTexture()->FullSize();								// 배경 크기 설정

	// 앞 배경
	std::shared_ptr<Actor> frontground = CreateActor();
	frontground->AddComponent<MeshRendererComponent>();
	frontground->SetName("FrontGround");

	auto frontgroundTexture = frontground->AddComponent<BackGroundComponent>();
	
	frontgroundTexture->SetTexture("Assets/Texture/Street1.png");
	frontgroundTexture->GetTexture()->FullSize(false, 960.0f, 480.0f);

	frontground->GetComponent<TransformComponent>()->SetScale(D3DXVECTOR3(WorldX, WorldY, 1.0f));
	frontground->GetComponent<TransformComponent>()->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// 왼쪽 빨간 집
	std::shared_ptr<Actor> leftRedHouse = CreateActor();
	leftRedHouse->AddComponent<MeshRendererComponent>();
	leftRedHouse->SetName("LeftRedHouse");

	auto leftRedHouseTexture = leftRedHouse->AddComponent<BackGroundComponent>();

	leftRedHouseTexture->SetTexture("Assets/Texture/LeftRedHouse.png");
	leftRedHouseTexture->GetTexture()->FullSize(false, 224.0f, 97.0f);

	leftRedHouse->GetComponent<TransformComponent>()->SetScale(D3DXVECTOR3(525.0f, 226.8f, 1.0f));
	leftRedHouse->GetComponent<TransformComponent>()->SetPosition(D3DXVECTOR3(-862.5f, 449.1f, 0.0f));

	// 오른쪽 빨간 집
	std::shared_ptr<Actor> rightRedHouse = CreateActor();
	rightRedHouse->AddComponent<MeshRendererComponent>();
	rightRedHouse->SetName("RightRedHouse");

	auto rightRedHouseTexture = rightRedHouse->AddComponent<BackGroundComponent>();

	rightRedHouseTexture->SetTexture("Assets/Texture/RightRedHouse.png");
	rightRedHouseTexture->GetTexture()->FullSize(false, 288.0f, 96.0f);

	rightRedHouse->GetComponent<TransformComponent>()->SetScale(D3DXVECTOR3(675.0f, 226.0f, 1.0f));
	rightRedHouse->GetComponent<TransformComponent>()->SetPosition(D3DXVECTOR3(-37.0f, 451.1f, 0.0f));

	// 왼쪽 문
	std::shared_ptr<Actor> leftDoor = CreateActor();
	leftDoor->AddComponent<MeshRendererComponent>();

	auto leftDoorTexture = leftDoor->AddComponent<AnimatorComponent>();
	leftDoorTexture->SetAnimationFrameName(1, "WoodDoor");
	leftDoorTexture->AddAnimation("Assets/Animation/Map/Door.xml");
	leftDoor->SetName("LeftWoodDoor");

	leftDoorTexture->SetCurrentAnimation("WoodDoor");
	leftDoorTexture->SetAnimationMode(AnimationMode::Stop);

	leftDoor->GetComponent<TransformComponent>()->SetScale(D3DXVECTOR3(70.0f, 120.0f, 1.0f));
	leftDoor->GetComponent<TransformComponent>()->SetPosition(D3DXVECTOR3(-1090.0f, 396.0f, 1.0f));

	// 오른쪽 문
	std::shared_ptr<Actor> rightDoor = CreateActor();
	rightDoor->AddComponent<MeshRendererComponent>();

	auto rightDoorTexture = rightDoor->AddComponent<AnimatorComponent>();
	rightDoorTexture->SetAnimationFrameName(1, "WoodDoor");
	rightDoorTexture->AddAnimation("Assets/Animation/Map/Door.xml");
	rightDoor->SetName("RightWoodDoor");

	rightDoorTexture->SetCurrentAnimation("WoodDoor");
	rightDoorTexture->SetAnimationMode(AnimationMode::Stop);

	rightDoor->GetComponent<TransformComponent>()->SetScale(D3DXVECTOR3(70.0f, 120.0f, 1.0f));
	rightDoor->GetComponent<TransformComponent>()->SetPosition(D3DXVECTOR3(-45.0f, 396.0f, 1.0f));

	// 3번째 울타리
	std::shared_ptr<Actor> fence3 = CreateActor();
	fence3->AddComponent<MeshRendererComponent>();
	fence3->SetName("fence3");

	auto fence3Texture = fence3->AddComponent<BackGroundComponent>();

	fence3Texture->SetTexture("Assets/Texture/fence3.png");
	fence3Texture->GetTexture()->FullSize(false, 14.0f, 159.0f);

	fence3->GetComponent<TransformComponent>()->SetScale(D3DXVECTOR3(35.0f, 375.0f, 1.0f));
	fence3->GetComponent<TransformComponent>()->SetPosition(D3DXVECTOR3(-483.0f, 375.0f, 0.0f));

	// 4번째 울타리
	std::shared_ptr<Actor> fence4 = CreateActor();
	fence4->AddComponent<MeshRendererComponent>();
	fence4->SetName("fence4");

	auto fence4Texture = fence4->AddComponent<BackGroundComponent>();

	fence4Texture->SetTexture("Assets/Texture/fence3.png");
	fence4Texture->GetTexture()->FullSize(false, 14.0f, 159.0f);

	fence4->GetComponent<TransformComponent>()->SetScale(D3DXVECTOR3(35.0f, 375.0f, 1.0f));
	fence4->GetComponent<TransformComponent>()->SetPosition(D3DXVECTOR3(488.0f, 375.0f, 0.0f));

	// 1번째 울타리
	std::shared_ptr<Actor> fence1 = CreateActor();
	fence1->AddComponent<MeshRendererComponent>();
	fence1->SetName("fance");

	auto fence1Texture = fence1->AddComponent<BackGroundComponent>();

	fence1Texture->SetTexture("Assets/Texture/fence.png");
	fence1Texture->GetTexture()->FullSize(false, 362.0f, 31.0f);

	fence1->GetComponent<TransformComponent>()->SetScale(D3DXVECTOR3(900.0f, 75.0f, 1.0f));
	fence1->GetComponent<TransformComponent>()->SetPosition(D3DXVECTOR3(-600.0f, 150.0f, 0.0f));

	// 2번째 울타리
	std::shared_ptr<Actor> fence2 = CreateActor();
	fence2->AddComponent<MeshRendererComponent>();
	fence2->SetName("fence2");

	auto fence2Texture = fence2->AddComponent<BackGroundComponent>();

	fence2Texture->SetTexture("Assets/Texture/fence2.png");
	fence2Texture->GetTexture()->FullSize(false, 217.0f, 32.0f);

	fence2->GetComponent<TransformComponent>()->SetScale(D3DXVECTOR3(517.0f, 75.0f, 1.0f));
	fence2->GetComponent<TransformComponent>()->SetPosition(D3DXVECTOR3(250.0f, 150.0f, 0.0f));

	// ================================================================
	//							실제 객체
	// ================================================================

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

	for (int i = 0; i < 10; i++)
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

	for (int i = 0; i < 10; i++)
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

	// ================================================================
	//				배경 객체 중 실재 객체를 안보이게 할 객체
	// ================================================================

	// 가로등
	D3DXVECTOR2 streetLampPos[7] =
	{
		D3DXVECTOR2(832.0f, 217.0f), D3DXVECTOR2(739.0f, 298.0f), D3DXVECTOR2(579.0f, 298.0f),
		D3DXVECTOR2(419.0f, 298.0f), D3DXVECTOR2(259.0f, 298.0f), D3DXVECTOR2(99.0f, 298.0f),
		D3DXVECTOR2(835.0f, 74.0f)
	};

	for (int i = 0; i < 7; i++)
	{
		streetLampPos[i] = D3DXVECTOR2(WINDOWTODIRECTXPOS(streetLampPos[i].x, 960.0f, 2250.0f, 64.0f), -WINDOWTODIRECTXPOS(streetLampPos[i].y, 480.0f, 1125.0f, 64.0f));

		std::shared_ptr<Actor> object = CreateActor();
		object->AddComponent<MeshRendererComponent>();

		auto texture = object->AddComponent<AnimatorComponent>();
		texture->SetAnimationFrameName(1, "StreetLamp");
		texture->AddAnimation("Assets/Animation/Map/StreetLamp.xml");
		object->SetName("StreetLamp");

		texture->SetCurrentAnimation("StreetLamp");
		texture->SetAnimationMode(AnimationMode::Stop);

		object->GetComponent<TransformComponent>()->SetScale(D3DXVECTOR3(50.0f, 150.0f, 1.0f));
		object->GetComponent<TransformComponent>()->SetPosition(D3DXVECTOR3(streetLampPos[i].x, streetLampPos[i].y, 1.0f));

		// 플레이어와 충돌 처리 영역 설정
		player->GetComponent<CollisionComponent>()->SetCollistionTarget(object);
		player->GetComponent<CollisionComponent>()->SetCollisionArea(StartArea::Bottom, 0.2f);

		// 몬스터와 충돌 처리 영역 설정
		for (const auto& mon : monster)
		{
			mon->GetComponent<CollisionComponent>()->SetCollistionTarget(object);
			mon->GetComponent<CollisionComponent>()->SetCollisionArea(StartArea::Bottom, 0.2f);
		}
	}

	// 나무 오브젝트
	D3DXVECTOR2 treePos[10] =
	{
		D3DXVECTOR2(32.0f, 384.0f), D3DXVECTOR2(352.0f, 352.0f), D3DXVECTOR2(448.0f, 320.0f),
		D3DXVECTOR2(448.0f, 416.0f), D3DXVECTOR2(576.0f, 416.0f), D3DXVECTOR2(640.0f, 320.0f),
		D3DXVECTOR2(704.0f, 416.0f), D3DXVECTOR2(768.0f, 384.0f), D3DXVECTOR2(896.0f, 160.0f), D3DXVECTOR2(704.0f, 64.0f)
	};

	for (int i = 0; i < 10; i++)
	{
		treePos[i] = D3DXVECTOR2(WINDOWTODIRECTXPOS(treePos[i].x, 960.0f, 2250.0f, 64.0f), -WINDOWTODIRECTXPOS(treePos[i].y, 480.0f, 1125.0f, 64.0f));

		std::shared_ptr<Actor> object = CreateActor();
		object->AddComponent<MeshRendererComponent>();

		auto texture = object->AddComponent<AnimatorComponent>();
		texture->SetAnimationFrameName(1, "NormalTree");
		texture->AddAnimation("Assets/Animation/Map/Tree.xml");
		object->SetName("Tree");

		texture->SetCurrentAnimation("NormalTree");
		texture->SetAnimationMode(AnimationMode::Stop);

		object->GetComponent<TransformComponent>()->SetScale(D3DXVECTOR3(150.0f, 200.0f, 1.0f));
		object->GetComponent<TransformComponent>()->SetPosition(D3DXVECTOR3(treePos[i].x, treePos[i].y, 1.0f));

		// 플레이어와 충돌 처리 영역 설정
		player->GetComponent<CollisionComponent>()->SetCollistionTarget(object);
		player->GetComponent<CollisionComponent>()->SetCollisionArea(StartArea::Bottom, 0.2f);

		// 몬스터와 충돌 처리 영역 설정
		for (const auto& mon : monster)
		{
			mon->GetComponent<CollisionComponent>()->SetCollistionTarget(object);
			mon->GetComponent<CollisionComponent>()->SetCollisionArea(StartArea::Bottom, 0.2f);
		}
	}

	// ================================================================
	//						생성뒤 처리할 데이터
	// ================================================================

	camera->GetComponent<TraceComponent>()->SetTarget(player);

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

	player->GetComponent<CollisionComponent>()->SetCollistionTarget(leftRedHouse);
	player->GetComponent<CollisionComponent>()->SetCollisionArea(StartArea::Top, 0.8f);

	player->GetComponent<CollisionComponent>()->SetCollistionTarget(rightRedHouse);
	player->GetComponent<CollisionComponent>()->SetCollisionArea(StartArea::Top, 0.8f);

	player->GetComponent<CollisionComponent>()->SetCollistionTarget(leftDoor);
	player->GetComponent<CollisionComponent>()->SetCollisionArea(StartArea::Top, 1.0f);

	player->GetComponent<CollisionComponent>()->SetCollistionTarget(rightDoor);
	player->GetComponent<CollisionComponent>()->SetCollisionArea(StartArea::Top, 1.0f);	

	player->GetComponent<CollisionComponent>()->SetCollistionTarget(fence1);
	player->GetComponent<CollisionComponent>()->SetCollisionArea(StartArea::Top, 0.05f);

	player->GetComponent<CollisionComponent>()->SetCollistionTarget(fence2);
	player->GetComponent<CollisionComponent>()->SetCollisionArea(StartArea::Top, 0.05f);

	player->GetComponent<CollisionComponent>()->SetCollistionTarget(fence3);
	player->GetComponent<CollisionComponent>()->SetCollisionArea(StartArea::Top, 1.0f);

	player->GetComponent<CollisionComponent>()->SetCollistionTarget(fence4);
	player->GetComponent<CollisionComponent>()->SetCollisionArea(StartArea::Top, 1.0f);

	for (const auto& object : monster)
	{
		object->GetComponent<CollisionComponent>()->SetCollistionTarget(leftRedHouse);
		object->GetComponent<CollisionComponent>()->SetCollisionArea(StartArea::Top, 0.8f);

		object->GetComponent<CollisionComponent>()->SetCollistionTarget(rightRedHouse);
		object->GetComponent<CollisionComponent>()->SetCollisionArea(StartArea::Top, 0.8f);

		object->GetComponent<CollisionComponent>()->SetCollistionTarget(leftDoor);
		object->GetComponent<CollisionComponent>()->SetCollisionArea(StartArea::Top, 1.0f);

		object->GetComponent<CollisionComponent>()->SetCollistionTarget(rightDoor);
		object->GetComponent<CollisionComponent>()->SetCollisionArea(StartArea::Top, 1.0f);

		object->GetComponent<CollisionComponent>()->SetCollistionTarget(fence1);
		object->GetComponent<CollisionComponent>()->SetCollisionArea(StartArea::Top, 0.05f);

		object->GetComponent<CollisionComponent>()->SetCollistionTarget(fence2);
		object->GetComponent<CollisionComponent>()->SetCollisionArea(StartArea::Top, 0.05f);

		object->GetComponent<CollisionComponent>()->SetCollistionTarget(fence3);
		object->GetComponent<CollisionComponent>()->SetCollisionArea(StartArea::Top, 1.0f);
	}

	std::queue<int> key; key.push(DIK_1); key.push(DIK_2);
	std::queue<PlayerBulletStyle> bulletStyle; bulletStyle.emplace(PlayerBulletStyle::Short); bulletStyle.emplace(PlayerBulletStyle::Long);
	std::queue<float> bulletSpeed; bulletSpeed.emplace(40.0f);

	ChangeAttackTypeManager::ChangeType(player, key, bulletStyle, bulletSpeed);
}

Scene::~Scene()
{
	actors.clear();
}

void Scene::Update()
{
	// 몬스터 1개씩 활성화
	if ((onMonster < 10) && (nowDelay < 0))
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

		onMonster++;
		nowDelay = 0.25f;
	}
	else nowDelay -= timer->GetDeltaTimeSEC();

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
	if(std::find(actors.begin(), actors.end(), actor) == actors.end())
	{
		assert(false && "ERROR (Scene1) : 해당 하는 actor가 존재하지 않음");
		return;
	}
	
	actors.erase(std::find(actors.begin(), actors.end(), actor));
	actor->~Actor();
}