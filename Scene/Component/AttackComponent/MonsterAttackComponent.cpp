#include "stdafx.h"
#include "MonsterAttackComponent.h"

#include "Scene/Actor.h"

#include "../MoveComponent/TraceComponent.h"
#include "../MoveComponent/TransformComponent.h"

#include "../AnimationComponent/AnimatorComponent.h"
#include "../AnimationComponent/GaugeBarComponent.h"

void MonsterAttackComponent::Initialize()
{
	multipleSpriteSize = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
}

void MonsterAttackComponent::Update()
{
	if (!actor->HasCompoent<TraceComponent>())
	{
		assert(false && "ERROR (TraceComponent1) : 해당 객체에 TraceComponent가 존재하지 않음");
		return;
	}

	if (actor->GetComponent<TraceComponent>()->GetTarget().lock() == nullptr || actor->GetComponent<TraceComponent>()->GetTarget().expired())
	{
		assert(false && "ERROR (TraceComponent2) : TraceComponent에 타겟을 지정하지 않거나 만료됨");
		return;
	}

	if (attackRange <= 0)
	{
		assert(false && "ERROR (TraceComponent3) : 해당 객체에 공격 범위를 지정하지 않거나 음수로 지정함");
		return;
	}

	if (damage <= 0)
	{
		assert(false && "ERROR (TraceComponent4) : 해당 객체에 공격력을 지정하지 않거나 음수로 지정함");
		return;
	}

	// 만약 몬스터가 죽었을 경우 아래는 처리하지 않음
	if (actor->GetComponent<TransformComponent>()->GetChildFromIndex(0).lock()->GetActor()->HasCompoent<GaugeBarComponent>() 
	 && actor->GetComponent<TransformComponent>()->GetChildFromIndex(0).lock()->GetActor()->GetComponent<GaugeBarComponent>()->GetIsDead())
		return;

	// TransformComponent에서 설정한 타겟을 가져옴
	auto target = actor->GetComponent<TraceComponent>()->GetTarget().lock();

	// 타겟 간의 거리를 구함
	D3DXVECTOR3 targetDisCoord = target->GetComponent<TransformComponent>()->GetPosition() - actor->GetComponent<TransformComponent>()->GetPosition();
	float distance = sqrt(pow(targetDisCoord.x, 2) + pow(targetDisCoord.y, 2));

	// FrameName을 가져오고 vector의 인덱스를 받을 변수를 선언
	auto frameName = actor->GetComponent<AnimatorComponent>()->GetNowKeyFrameName();		// 현재의 frameName을 가져옴
	auto allFrameName = actor->GetComponent<AnimatorComponent>()->GetAnimationFrameNames();	// 객체에 저장되어 있는 모든 FrameName을 가져옴

	auto iter = allFrameName.end();
	isAttack = false;

	// 사격 거리 안에 들어온 경우
	if (distance <= attackRange)
	{
		if (!frameName.compare("RightMove"))
		{	
			// 오른쪽을 보고 있는 상태에서 공격하는 스프라이트를 가져옴
			iter = std::find(allFrameName.begin(), allFrameName.end(), "RightAttack");

			if (iter == allFrameName.end())
			{
				assert(false && "ERROR (AttakComponent1) : 해당 객체에 \"RightAttack\" Frame Name이 없음");
				return;
			}
		}

		else if (!frameName.compare("LeftMove"))
		{
			// 왼쪽을 보고 있는 상태에서 공격하는 스프라이트를 가져옴
			iter = std::find(allFrameName.begin(), allFrameName.end(), "LeftAttack");

			if (iter == allFrameName.end())
			{
				assert(false && "ERROR (AttackComponet2 : 해당 객체에 \"LeftAttack\" Frame Name이 없음");
				return;
			}
		}

		isAttack = true;

		if (isMulSize && !nowMulSize)
		{
			// 공격 스프라이트의 크기로 인하여 객체를 해당 크기로 증가시킴 
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

		// 스프라이트가 없을 경우 오류
		if (iter == allFrameName.end()) return;

		// 가져온 인덱스를 싫행 시킴 (단, 다른 스프라이트가 실행 중일 때만)
		if (actor->GetComponent<AnimatorComponent>()->GetNowKeyFrameName() != *iter)
			actor->GetComponent<AnimatorComponent>()->SetCurrentAnimation(*iter);
	}

	else
	{
		if (isMulSize && nowMulSize)
		{
			// 공격 스프라이트의 크기로 인하여 객체를 해당 크기로 증가시킴 
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
