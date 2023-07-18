#include "stdafx.h"
#include "TraceComponent.h"
#include "TransformComponent.h"
#include "CollisionComponent.h"

#include "../AnimationComponent/AnimatorComponent.h"
#include "../AttackComponent/MonsterAttackComponent.h"

#include "Intersect/Intersect.h"
#include "Scene/Actor.h"

// ���� ī�޶�� ��� ��
#define WATINGTIME 2		// ������ ������ �浹�ϴ� �ð�
#define ESCAPESPEED 4		// Ÿ���� �������� �ӵ� 

void TraceComponent::Initialize() { }

void TraceComponent::Update()
{
	targetPos = target.lock()->GetTransform()->GetPosition();			// Ÿ�� ��ġ�� ������
	targetSize = target.lock()->GetTransform()->GetScale();				// Ÿ�� ũ�⸦ ������

	D3DXVECTOR3 diffPos = targetPos - transform->GetPosition();			// �ش� ��ü�� Ÿ�� ��ġ�� �� ��
	D3DXVECTOR3 moveTo;													// �̵��� ��ġ
	float speedX = 0, speedY = 0;										// �̵� �ӵ�
	float MoveToX = 0, MoveToY = 0;

	// ���� ī�޶� �� ���
	if (actor->GetName() == "MainCamera")
	{
		D3DXVECTOR3 pos = actor->GetComponent<TransformComponent>()->GetPosition();

		// �̵� �ӵ� ���� = (�Ÿ����� * ������) / (ȭ��ũ�� * �浹�ϴµ� �ɸ��� �ð�)
		speedX = ((diffPos.x * 60) / (Settings::Get().GetWidth() * WATINGTIME));
		speedY = ((diffPos.y * 60) / (Settings::Get().GetHeight() * WATINGTIME));

		if (pos.x + speedX < ((-WorldX / 2) + (Settings::Get().GetWidth() / 2)) || pos.x + speedX > ((WorldX / 2) - (Settings::Get().GetWidth() / 2)))
			speedX = 0.0f;

		if (pos.y + speedY < ((-WorldY / 2) + (Settings::Get().GetHeight() / 2)) || pos.y + speedY > ((WorldY / 2) - (Settings::Get().GetHeight() / 2)))
			speedY = 0.0f;
	}

	// �Ϲ����� ��ü�� ���
	else
	{
		if (speed == 0.0f)
		{
			assert(false && "Error (TraceComponent1) : �ӵ��� �Է����� �ʾ���");
			return;
		}

		if (!actor->HasCompoent<AnimatorComponent>())
		{
			assert(false && "Error (TraceComponent2) : �ش� ��ü�� �ִϸ��̼��� ���� �ʾ���");
			return;
		}

		if (!actor->HasCompoent<MonsterAttackComponent>())
		{
			assert(false && "Error (TraceComponent3) : �ش� ��ü�� MonsterAttackComponent�� �������� ����");
			return;
		}


		// �̵������� �ٲ�� ���� �����ϴ� ������ �ٲٱ� ���� ��� ������ Ǯ��
		if (diffPos.x > 0 && direction == Direction::Left) actor->GetComponent<MonsterAttackComponent>()->SetIsAttack(false);
		if (diffPos.x < 0 && direction == Direction::Right) actor->GetComponent<MonsterAttackComponent>()->SetIsAttack(false);

		// �ش� ��ü�� ���� ���̸� ����
		if (actor->GetComponent<MonsterAttackComponent>()->GetIsAttack()) return;

		// �̵� ���� �� �̵� �ӵ��� ����
		diffPos.y -= (targetSize.y / 2 - actor->GetComponent<TransformComponent>()->GetScale().y / 2);	// ���� ��ü�� �Ʒ��� �̵��ϰ� ���� (�������� �ʾ��� ��� ���Ͱ� ���߿� �ߴ� ������ ����)

		diffPos.x /= static_cast<float>(sqrt(pow(diffPos.x, 2) + pow(diffPos.y, 2)));					// �̵� ������ ����
		diffPos.y /= static_cast<float>(sqrt(pow(diffPos.x, 2) + pow(diffPos.y, 2)));

		speedX = ((diffPos.x * speed * 60) / Settings::Get().GetWidth());								// �� ���� ���� �̵� �ӵ��� ����
		speedY = ((diffPos.y * speed * 60) / Settings::Get().GetHeight());

		// ��ġ�� ���� ���� ����
		toPos = D3DXVECTOR2(speedX, speedY);							

		// ��������Ʈ 
		std::vector<std::string> spriteName = actor->GetComponent<AnimatorComponent>()->GetAnimationFrameNames(); // ��� �ִϸ��̼��� ������ �̸��� ������

		if (diffPos.x > 0)
		{
			// ���������� �̵��ϴ� ��������Ʈ�� ã��
			auto iter = std::find_if(spriteName.begin(), spriteName.end(), FindRightMoveSprite);

			if (iter == spriteName.end())
			{
				assert(false && "Eror (TraceComponent3) : �ش� ��ü�� \"RightMove\" Frame Name�� ����");
				return;
			}

			// �ش� ��������Ʈ�� �Է�
			if(actor->GetComponent<AnimatorComponent>()->GetNowKeyFrameName() != *iter) 
				actor->GetComponent<AnimatorComponent>()->SetCurrentAnimation(*iter);

			direction = Direction::Right;
		}

		else if (diffPos.x < 0)
		{
			// �������� �̵��ϴ� ��������Ʈ�� ã��
			std::vector<std::string>::iterator iter = std::find_if(spriteName.begin(), spriteName.end(), FindLeftMoveSprite);

			if (iter == spriteName.end())
			{
				assert(false && "Eror (TraceComponent4) : �ش� ��ü�� \"LeftMove\" Frame Name�� ����");
				return;
			}

			// �ش� ��������Ʈ�� �Է�
			if (actor->GetComponent<AnimatorComponent>()->GetNowKeyFrameName() != *iter)
				actor->GetComponent<AnimatorComponent>()->SetCurrentAnimation(*iter);

			direction = Direction::Left;
		}
	}

	// ���� ��� �浹 ��ü�� �浹�� ��� �̵����� ����
	if (actor->HasCompoent<CollisionComponent>() && actor->GetComponent<CollisionComponent>()->IsCollision()) 
		return;

	MoveToX = transform->GetPosition().x + speedX;
	MoveToY = transform->GetPosition().y + speedY;

	moveTo = D3DXVECTOR3(MoveToX, MoveToY, transform->GetPosition().z);	// �̵��� ��ġ ����

	transform->SetPosition(moveTo);										// �ش� ��ġ�� �̵�
}

void TraceComponent::Destroy()
{
}
