#include "stdafx.h"
#include "MonsterAttackComponent.h"

#include "Scene/Actor.h"

#include "../MoveComponent/TraceComponent.h"
#include "../MoveComponent/TransformComponent.h"

#include "../AnimationComponent/AnimatorComponent.h"

void MonsterAttackComponent::Initialize()
{
	multipleSpriteSize = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
}

void MonsterAttackComponent::Update()
{
	if (!actor->HasCompoent<TraceComponent>())
	{
		assert(false && "ERROR (TraceComponent1) : �ش� ��ü�� TraceComponent�� �������� ����");
		return;
	}

	if (actor->GetComponent<TraceComponent>()->GetTarget().lock() == nullptr || actor->GetComponent<TraceComponent>()->GetTarget().expired())
	{
		assert(false && "ERROR (TraceComponent2) : TraceComponent�� Ÿ���� �������� �ʰų� �����");
		return;
	}

	if (attackRange <= 0)
	{
		assert(false && "ERROR (TraceComponent3) : �ش� ��ü�� ���� ������ �������� �ʰų� ������ ������");
		return;
	}

	if (damage <= 0)
	{
		assert(false && "ERROR (TraceComponent4) : �ش� ��ü�� ���ݷ��� �������� �ʰų� ������ ������");
		return;
	}

	// TransformComponent���� ������ Ÿ���� ������
	auto target = actor->GetComponent<TraceComponent>()->GetTarget().lock();

	// Ÿ�� ���� �Ÿ��� ����
	D3DXVECTOR3 targetDisCoord = target->GetComponent<TransformComponent>()->GetPosition() - actor->GetComponent<TransformComponent>()->GetPosition();
	float distance = sqrt(pow(targetDisCoord.x, 2) + pow(targetDisCoord.y, 2));

	// FrameName�� �������� vector�� �ε����� ���� ������ ����
	auto frameName = actor->GetComponent<AnimatorComponent>()->GetNowKeyFrameName();		// ������ frameName�� ������
	auto allFrameName = actor->GetComponent<AnimatorComponent>()->GetAnimationFrameNames();	// ��ü�� ����Ǿ� �ִ� ��� FrameName�� ������

	auto iter = allFrameName.end();
	isAttack = false;

	// ��� �Ÿ� �ȿ� ���� ���
	if (distance <= attackRange)
	{
		if (frameName.size() >= 9 && frameName.substr(frameName.size() - 9, 9) == "RightMove")
		{	
			// �������� ���� �ִ� ���¿��� �����ϴ� ��������Ʈ�� ������
			iter = std::find_if(allFrameName.begin(), allFrameName.end(), FindRightAttackSprite);

			if (iter == allFrameName.end())
			{
				assert(false && "ERROR (AttakComponent1) : �ش� ��ü�� \"RightAttack\" Frame Name�� ����");
				return;
			}
		}

		else if (frameName.size() >= 8 && frameName.substr(frameName.size() - 8, 8) == "LeftMove")
		{
			// ������ ���� �ִ� ���¿��� �����ϴ� ��������Ʈ�� ������
			iter = std::find_if(allFrameName.begin(), allFrameName.end(), FindLeftAttackSprite);

			if (iter == allFrameName.end())
			{
				assert(false && "ERROR (AttackComponet2 : �ش� ��ü�� \"LeftAttack\" Frame Name�� ����");
				return;
			}
		}

		isAttack = true;

		if (isMulSize && !nowMulSize)
		{
			// ���� ��������Ʈ�� ũ��� ���Ͽ� ��ü�� �ش� ũ��� ������Ŵ 
			const D3DXVECTOR3 scale = actor->GetComponent<TransformComponent>()->GetScale();

			switch (axis)
			{
			case ResizeTo::X:
				actor->GetComponent<TransformComponent>()->SetScale(D3DXVECTOR3(scale.x * multipleSpriteSize.x, scale.y, scale.z));
				break;

			case ResizeTo::Y:
				actor->GetComponent<TransformComponent>()->SetScale(D3DXVECTOR3(scale.x, scale.y * multipleSpriteSize.y, scale.z));
				break;

			default:
				break;
			}

			nowMulSize = true;
		}

		// ��������Ʈ�� ���� ��� ����
		if (iter == allFrameName.end()) return;

		// ������ �ε����� ���� ��Ŵ (��, �ٸ� ��������Ʈ�� ���� ���� ����)
		if (actor->GetComponent<AnimatorComponent>()->GetNowKeyFrameName() != *iter)
			actor->GetComponent<AnimatorComponent>()->SetCurrentAnimation(*iter);
	}

	else
	{
		if (isMulSize && nowMulSize)
		{
			// ���� ��������Ʈ�� ũ��� ���Ͽ� ��ü�� �ش� ũ��� ������Ŵ 
			const D3DXVECTOR3 scale = actor->GetComponent<TransformComponent>()->GetScale();

			switch (axis)
			{
			case ResizeTo::X:
				actor->GetComponent<TransformComponent>()->SetScale(D3DXVECTOR3(scale.x / multipleSpriteSize.x, scale.y, scale.z));
				break;

			case ResizeTo::Y:
				actor->GetComponent<TransformComponent>()->SetScale(D3DXVECTOR3(scale.x, scale.y / multipleSpriteSize.y, scale.z));
				break;

			default:
				break;
			}

			nowMulSize = false;
		}
	}
}

void MonsterAttackComponent::Destroy()
{
}
