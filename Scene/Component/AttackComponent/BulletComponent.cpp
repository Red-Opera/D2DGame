#include "stdafx.h"
#include "BulletComponent.h"

#include "../../Actor.h"
#include "../MeshRendererComponent.h"
#include "../AnimationComponent/AnimatorComponent.h"
#include "../AnimationComponent/GaugeBarComponent.h"
#include "../AttackComponent/MonsterAttackComponent.h"
#include "../MoveComponent/TraceComponent.h"
#include "../MoveComponent/TransformComponent.h"
#include "../MoveComponent/CollisionComponent.h"

#include "Scene/Scene.h"

void BulletComponent::PossibleProceed()
{
	if (!actor->HasCompoent<TraceComponent>())
	{
		assert(false && "ERROR (BulletComponent1) : 해당 객체에 TraceComponent가 존재하지 않음");
		return;
	}

	if (!actor->HasCompoent<TransformComponent>())
	{
		assert(false && "ERROR (BulletComponent2) : 해당 객체에 TransformComponent가 존재하지 않음");
		return;
	}

	if (actor->GetComponent<TraceComponent>()->GetTarget().lock() == nullptr || actor->GetComponent<TraceComponent>()->GetTarget().expired())
	{
		assert(false && "ERROR (BulletComponent3) : TraceComponent에 타겟을 지정하지 않거나 만료됨");
		return;
	}

	if (bulletStyle == BulletStyle::None)
	{
		assert(false && "ERROR (BulletComponent4) : 객체의 공격 형태를 지정하지 않음");
		return;
	}

	if (speed <= 0)
	{
		assert(false && "ERROR (BulletComponent5) : 총알 스피드를 잘못 지정하거나 입력하지 않음");
		return;
	}

	if ((bulletStyle == BulletStyle::Long) && (spriteFrameName == "" || spritePath == ""))
	{
		assert(false && "ERROR (BulletComponent6) : 스프라이트 이름을 지정하지 않았거나 경로가 잘못되었음");
		return;
	}
}

void BulletComponent::Initialize()
{
	timer = context->GetSubSystem<Timer>();
}

void BulletComponent::Update()
{
	PossibleProceed();

	// 타겟을 불러옴
	auto target = actor->GetComponent<TraceComponent>()->GetTarget().lock();

	// 각각의 위치를 구함
	D3DXVECTOR3 pos = D3DXVECTOR3(actor->GetComponent<TransformComponent>()->GetPosition().x, actor->GetComponent<TransformComponent>()->GetPosition().y, 1);
	D3DXVECTOR3 tarPos = D3DXVECTOR3(target->GetComponent<TransformComponent>()->GetPosition().x, target->GetComponent<TransformComponent>()->GetPosition().y, 1);

	float dis = sqrt(pow((pos.x - tarPos.x), 2) + pow((pos.y - tarPos.y), 2));

	// 총알 생성
	if (nowDelay <= 0 && actor->GetComponent<MonsterAttackComponent>()->GetIsAttack())
	{
		switch (bulletStyle)
		{
		case BulletStyle::Long:
		{
			bullets.emplace_back(actor->GetScene()->CreateActor());

			bullets.back()->AddComponent<MeshRendererComponent>();
			bullets.back()->AddComponent<TransformComponent>();
			bullets.back()->AddComponent<CollisionComponent>();

			bullets.back()->GetComponent<TransformComponent>()->SetParent(actor->GetComponent<TransformComponent>());
			actor->GetComponent<TransformComponent>()->AddChild(bullets.back()->GetComponent<TransformComponent>());

			auto texture = bullets.back()->AddComponent<AnimatorComponent>();
			texture->SetAnimationFrameName(1, spriteFrameName.c_str());
			texture->AddAnimation(spritePath.c_str());
			bullets.back()->SetName(spriteFrameName);

			texture->SetCurrentAnimation(spriteFrameName);
			texture->SetAnimationMode(AnimationMode::Play);

			bullets.back()->GetComponent<TransformComponent>()->SetLocalPosition(D3DXVECTOR3(0, 0, 0));
			bullets.back()->GetComponent<TransformComponent>()->SetLocalScale(D3DXVECTOR3(1.0f, 0.5f, 1.0f));
			bullets.back()->GetComponent<TransformComponent>()->SetLocalRotation(D3DXVECTOR3(0, 0, atan2((pos.y - tarPos.y) / dis, (pos.x - tarPos.x)/ dis)));
			bullets.back()->GetComponent<TransformComponent>()->SetFront(D3DXVECTOR3((tarPos.x - pos.x) / dis, (tarPos.y - pos.y) / dis, 0));

			target->GetComponent<CollisionComponent>()->SetCollistionTarget(bullets.back());
			target->GetComponent<CollisionComponent>()->SetCollisionArea(StartArea::Bottom, 1.0f);

			nowDelay = actor->GetComponent<MonsterAttackComponent>()->GetAttackDelay();

			break;
		}

		case BulletStyle::Short:
		{
			// 타겟에 데미지를 줌
			auto toDamage = actor->GetScene()->GetActors().begin();

			for (; toDamage != actor->GetScene()->GetActors().end(); toDamage++)
			{
				if ((*toDamage)->GetName() == "GaugeBar")
					break;
			}

			(*toDamage)->GetComponent<GaugeBarComponent>()->SetDamegeHp(actor->GetComponent<MonsterAttackComponent>()->GetDamage());

			nowDelay = actor->GetComponent<MonsterAttackComponent>()->GetAttackDelay();

			return;
		}

		default:
			break;
		}
	}

	else nowDelay -= timer->GetDeltaTimeSEC();

	// 아래 코드는 Long만 사용하도록 설정
	if (bulletStyle == BulletStyle::Short) return;

	// 총알 이동
	for (auto iter = bullets.begin(); !bullets.empty() && iter != bullets.end(); iter++)
	{
		const auto& transform = (*iter)->GetComponent<TransformComponent>();

		// 원래 있는 위치에서 앞쪽 방향으로 속도만큼 전진 시킴
		transform->SetPosition(transform->GetPosition() + transform->GetFront() * speed);
	}

	// 범위 밖의 총알 삭제
	for (auto iter = bullets.begin(); iter != bullets.end();)
	{
		if ((*iter)->GetComponent<TransformComponent>()->GetPosition().x > WorldX / 2 || (*iter)->GetComponent<TransformComponent>()->GetPosition().x < -WorldX / 2 ||
			(*iter)->GetComponent<TransformComponent>()->GetPosition().y > WorldY / 2 || (*iter)->GetComponent<TransformComponent>()->GetPosition().y < -WorldY / 2)
		{
			(*iter)->Destroy();
			(*iter)->GetScene()->EreaseActor((*iter));

			iter = bullets.erase(iter);
		}

		else iter++;
	}

	// 데미지 처리
	auto toDamage = actor->GetScene()->GetActors().begin();

	for (; toDamage != actor->GetScene()->GetActors().end(); toDamage++)
	{
		if ((*toDamage)->GetName() == "GaugeBar")
			break;
	}

	// 충돌 처리
	if (target->GetComponent<CollisionComponent>()->IsCollision())
	{
		// 가장 가까운 총알을 검색
		auto collision = target->GetComponent<CollisionComponent>()->GetCollisionObject();

		for (auto& object : collision)
		{
			// 타겟이 충돌한 객체 중에서 일치하는 객체를 찾음
			auto clearTarget = std::find(bullets.begin(), bullets.end(), object.lock());

			if (clearTarget != bullets.end())
			{
				(*clearTarget)->Destroy();
				(*clearTarget)->GetScene()->EreaseActor((*clearTarget));
				bullets.erase(std::find(bullets.begin(), bullets.end(), object.lock()));

				(*toDamage)->GetComponent<GaugeBarComponent>()->SetDamegeHp(actor->GetComponent<MonsterAttackComponent>()->GetDamage());

				break;
			}
		}
	}
}

void BulletComponent::Destroy()
{
	
}