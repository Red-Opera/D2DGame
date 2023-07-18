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
		assert(false && "(Error PlayerSkillComponent2) : �÷��̾ PlayerAttackComponent�� ����");
		return;
	}
	
	// ������ �� ���
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

	// �ܰŸ� �� ��� ��ų ����� ������
	if (actor->GetComponent<PlayerAttackComponent>()->GetBulletStyle() == PlayerBulletStyle::Short) return;

	if ((HOLDPRESSKEYBOARD(input, DIK_C)) && (gaugeBarComponent->GetMana() > manaConsum) && (nowDelay <= 0))
	{
		for (int i = 0; i < 3; i++)
		{
			auto attackComponent = actor->GetComponent<PlayerAttackComponent>();

			// �ѿ� �Ѿ� �߰�
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

			POINT curPos = GetCursorSystem::mouse;												// ���콺 ��ġ
			float x = curPos.x * cos(PI * (1 - i) / 6) + curPos.y * sin(PI * (1 - i) / 6);		// ���� �Ѿ� ��ġ ��ȭ�� ���� ���� �Ѿ� ��ġ
			float y = -curPos.x * sin(PI * (1 - i) / 6) + curPos.y * cos(PI * (1 - i)/ 6);
			float curDiff = sqrt(pow(x, 2) + pow(y, 2));										// ���콺�� �÷��̾� ��ġ����

			attackComponent->GetBullets().back()->GetComponent<TransformComponent>()->SetPosition(D3DXVECTOR3(gunTransform->GetPosition().x + i * 10, gunTransform->GetPosition().y, 0));
			attackComponent->GetBullets().back()->GetComponent<TransformComponent>()->SetLocalScale(D3DXVECTOR3(0.2f, 0.2f, 1.0f));
			attackComponent->GetBullets().back()->GetComponent<TransformComponent>()->SetLocalRotation(D3DXVECTOR3(0.0f, 0.0f, atan2(y / curDiff, x / curDiff) - (PI / 2)));
			attackComponent->GetBullets().back()->GetComponent<TransformComponent>()->SetFront(D3DXVECTOR3(x / curDiff, y / curDiff, 0));

			// �Ѿ� ��������Ʈ�� ��� ���Ͱ� �浹�� �� �ֵ��� �浹Ÿ�� ����
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
