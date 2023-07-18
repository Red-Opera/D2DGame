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
		assert(false && "ERROR (BulletComponent1) : �ش� ��ü�� TraceComponent�� �������� ����");
		return;
	}

	if (!actor->HasCompoent<TransformComponent>())
	{
		assert(false && "ERROR (BulletComponent2) : �ش� ��ü�� TransformComponent�� �������� ����");
		return;
	}

	if (actor->GetComponent<TraceComponent>()->GetTarget().lock() == nullptr || actor->GetComponent<TraceComponent>()->GetTarget().expired())
	{
		assert(false && "ERROR (BulletComponent3) : TraceComponent�� Ÿ���� �������� �ʰų� �����");
		return;
	}

	if (bulletStyle == BulletStyle::None)
	{
		assert(false && "ERROR (BulletComponent4) : ��ü�� ���� ���¸� �������� ����");
		return;
	}

	if (speed <= 0)
	{
		assert(false && "ERROR (BulletComponent5) : �Ѿ� ���ǵ带 �߸� �����ϰų� �Է����� ����");
		return;
	}

	if ((bulletStyle == BulletStyle::Long) && (spriteFrameName == "" || spritePath == ""))
	{
		assert(false && "ERROR (BulletComponent6) : ��������Ʈ �̸��� �������� �ʾҰų� ��ΰ� �߸��Ǿ���");
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

	// Ÿ���� �ҷ���
	auto target = actor->GetComponent<TraceComponent>()->GetTarget().lock();

	// ������ ��ġ�� ����
	D3DXVECTOR3 pos = D3DXVECTOR3(actor->GetComponent<TransformComponent>()->GetPosition().x, actor->GetComponent<TransformComponent>()->GetPosition().y, 1);
	D3DXVECTOR3 tarPos = D3DXVECTOR3(target->GetComponent<TransformComponent>()->GetPosition().x, target->GetComponent<TransformComponent>()->GetPosition().y, 1);

	float dis = sqrt(pow((pos.x - tarPos.x), 2) + pow((pos.y - tarPos.y), 2));

	// �Ѿ� ����
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
			// Ÿ�ٿ� �������� ��
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

	// �Ʒ� �ڵ�� Long�� ����ϵ��� ����
	if (bulletStyle == BulletStyle::Short) return;

	// �Ѿ� �̵�
	for (auto iter = bullets.begin(); !bullets.empty() && iter != bullets.end(); iter++)
	{
		const auto& transform = (*iter)->GetComponent<TransformComponent>();

		// ���� �ִ� ��ġ���� ���� �������� �ӵ���ŭ ���� ��Ŵ
		transform->SetPosition(transform->GetPosition() + transform->GetFront() * speed);
	}

	// ���� ���� �Ѿ� ����
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

	// ������ ó��
	auto toDamage = actor->GetScene()->GetActors().begin();

	for (; toDamage != actor->GetScene()->GetActors().end(); toDamage++)
	{
		if ((*toDamage)->GetName() == "GaugeBar")
			break;
	}

	// �浹 ó��
	if (target->GetComponent<CollisionComponent>()->IsCollision())
	{
		// ���� ����� �Ѿ��� �˻�
		auto collision = target->GetComponent<CollisionComponent>()->GetCollisionObject();

		for (auto& object : collision)
		{
			// Ÿ���� �浹�� ��ü �߿��� ��ġ�ϴ� ��ü�� ã��
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