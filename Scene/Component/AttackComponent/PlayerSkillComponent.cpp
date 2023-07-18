#include "stdafx.h"
#include "PlayerSkillComponent.h"

#include "Scene/Scene.h" 

#include "../../Actor.h"
#include "../MeshRendererComponent.h"
#include "../AnimationComponent/AnimatorComponent.h"
#include "../AnimationComponent/GaugeBarComponent.h"
#include "../AttackComponent/PlayerAttackComponent.h"
#include "../MoveComponent/CollisionComponent.h"
#include "../MoveComponent/TransformComponent.h"

#define HOLDPRESSKEYBOARD(input, key) (input->GetKeyState(key) == KEY_HOLD || input->GetKeyState(key) == KEY_DOWN)
#define ONLYPRESSKEYBOARD(input, key) (input->GetKeyState(key) == KEY_DOWN)

#define HOLDPRESSMOUSE(input, key) (input->GetMouseState(key) == KEY_HOLD || input->GetMouseState(key) == KEY_DOWN)
#define ONLYPRESSMOUSE(input, key) (input->GetMouseState(key) == KEY_DOWN)

void PlayerSkillComponent::Initialize()
{
	const auto& childs = actor->GetComponent<TransformComponent>()->GetChilds();

	if (!actor->HasCompoent<PlayerAttackComponent>())
	{
		assert(false && "(Error PlayerSkillComponent2) : 플레이어에 PlayerAttackComponent가 없음");
		return;
	}
	
	// 게이지 바 등록
	for (const auto& object : actor->GetScene()->GetActors())
	{
		if(object->GetName() == "GaugeBar")
			gaugeBar = object;
	}

	timer = context->GetSubSystem<Timer>();
}

void PlayerSkillComponent::Update()
{
	auto input = context->GetSubSystem<InputManager>();
	const auto& gaugeBarComponent = gaugeBar->GetComponent<GaugeBarComponent>();

	// 단거리 일 경우 스킬 사용이 금지됨
	if (actor->GetComponent<PlayerAttackComponent>()->GetBulletStyle() == PlayerBulletStyle::Short) return;

	if ((HOLDPRESSKEYBOARD(input, DIK_C)) && (gaugeBarComponent->GetMana() > manaConsum) && (nowDelay <= 0))
	{
		for (int i = 0; i < 3; i++)
		{
			auto attackComponent = actor->GetComponent<PlayerAttackComponent>();

			// 총에 총알 추가
			attackComponent->AddBullet(actor->GetScene()->CreateActor());

			attackComponent->GetBullets().back()->AddComponent<MeshRendererComponent>();
			attackComponent->GetBullets().back()->AddComponent<TransformComponent>();
			attackComponent->GetBullets().back()->AddComponent<CollisionComponent>();

			attackComponent->GetBullets().back()->GetComponent<TransformComponent>()->SetParent(actor->GetComponent<TransformComponent>());
			actor->GetComponent<TransformComponent>()->AddChild(attackComponent->GetBullets().back()->GetComponent<TransformComponent>());

			auto texture = attackComponent->GetBullets().back()->AddComponent<AnimatorComponent>();
			texture->SetAnimationFrameName(1, "Bullet");
			texture->AddAnimation("Assets/Animation/Bullet/PlayerBulletShot.xml");
			attackComponent->GetBullets().back()->SetName("Bullet");

			texture->SetCurrentAnimation("Bullet");
			texture->SetAnimationMode(AnimationMode::Stop);

			const auto& gunTransform = actor->GetComponent<PlayerAttackComponent>()->GetGunTransform();

			POINT curPos = GetCursorSystem::mouse;												// 마우스 위치
			float x = curPos.x * cos(PI * (1 - i) / 6) + curPos.y * sin(PI * (1 - i) / 6);		// 각각 총알 위치 변화에 따른 최종 총알 위치
			float y = -curPos.x * sin(PI * (1 - i) / 6) + curPos.y * cos(PI * (1 - i)/ 6);
			float curDiff = sqrt(pow(x, 2) + pow(y, 2));										// 마우스와 플레이어 위치차이

			attackComponent->GetBullets().back()->GetComponent<TransformComponent>()->SetPosition(D3DXVECTOR3(gunTransform->GetPosition().x + i * 10, gunTransform->GetPosition().y, 0));
			attackComponent->GetBullets().back()->GetComponent<TransformComponent>()->SetLocalScale(D3DXVECTOR3(0.2f, 0.2f, 1.0f));
			attackComponent->GetBullets().back()->GetComponent<TransformComponent>()->SetLocalRotation(D3DXVECTOR3(0.0f, 0.0f, atan2(y / curDiff, x / curDiff) - (PI / 2)));
			attackComponent->GetBullets().back()->GetComponent<TransformComponent>()->SetFront(D3DXVECTOR3(x / curDiff, y / curDiff, 0));

			// 총알 스프라이트가 모든 몬스터가 충돌할 수 있도록 충돌타겟 설정
			for (const auto& object : actor->GetScene()->GetActors())
			{
				if (object->GetName().size() < 7) continue;

				if (object->GetName().substr(object->GetName().size() - 7, 7) == "Monster")
				{
					attackComponent->GetBullets().back()->GetComponent<CollisionComponent>()->SetCollistionTarget(object);
					attackComponent->GetBullets().back()->GetComponent<CollisionComponent>()->SetCollisionArea(StartArea::Top, 1.0f);
				}
			}

			gaugeBarComponent->AddMana(-static_cast<float>(manaConsum));
		}

		nowDelay = 0.1f;
	}

	else if (gaugeBarComponent->GetMaxMana() > gaugeBarComponent->GetMana())
		gaugeBarComponent->AddMana(0.2f);

	else gaugeBarComponent->SetMana(gaugeBarComponent->GetMaxMana());

	if (nowDelay > 0.0f) nowDelay -= timer->GetDeltaTimeSEC();
}

void PlayerSkillComponent::Destroy()
{
}
