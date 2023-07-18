#include "stdafx.h"
#include "TraceComponent.h"
#include "TransformComponent.h"
#include "CollisionComponent.h"

#include "../AnimationComponent/AnimatorComponent.h"
#include "../AttackComponent/MonsterAttackComponent.h"

#include "Intersect/Intersect.h"
#include "Scene/Actor.h"

// 메인 카메라용 상수 값
#define WATINGTIME 2		// 가만히 있으면 충돌하는 시간
#define ESCAPESPEED 4		// 타겟이 떨어지는 속도 

void TraceComponent::Initialize() { }

void TraceComponent::Update()
{
	targetPos = target.lock()->GetTransform()->GetPosition();			// 타겟 위치를 가져옴
	targetSize = target.lock()->GetTransform()->GetScale();				// 타겟 크기를 가져옴

	D3DXVECTOR3 diffPos = targetPos - transform->GetPosition();			// 해당 물체와 타겟 위치를 비교 함
	D3DXVECTOR3 moveTo;													// 이동할 위치
	float speedX = 0, speedY = 0;										// 이동 속도
	float MoveToX = 0, MoveToY = 0;

	// 메인 카메라 일 경우
	if (actor->GetName() == "MainCamera")
	{
		D3DXVECTOR3 pos = actor->GetComponent<TransformComponent>()->GetPosition();

		// 이동 속도 공식 = (거리차이 * 프레임) / (화면크기 * 충돌하는데 걸리는 시간)
		speedX = ((diffPos.x * 60) / (Settings::Get().GetWidth() * WATINGTIME));
		speedY = ((diffPos.y * 60) / (Settings::Get().GetHeight() * WATINGTIME));

		if (pos.x + speedX < ((-WorldX / 2) + (Settings::Get().GetWidth() / 2)) || pos.x + speedX > ((WorldX / 2) - (Settings::Get().GetWidth() / 2)))
			speedX = 0.0f;

		if (pos.y + speedY < ((-WorldY / 2) + (Settings::Get().GetHeight() / 2)) || pos.y + speedY > ((WorldY / 2) - (Settings::Get().GetHeight() / 2)))
			speedY = 0.0f;
	}

	// 일반적인 객체인 경우
	else
	{
		if (speed == 0.0f)
		{
			assert(false && "Error (TraceComponent1) : 속도를 입력하지 않았음");
			return;
		}

		if (!actor->HasCompoent<AnimatorComponent>())
		{
			assert(false && "Error (TraceComponent2) : 해당 객체에 애니메이션을 넣지 않았음");
			return;
		}

		if (!actor->HasCompoent<MonsterAttackComponent>())
		{
			assert(false && "Error (TraceComponent3) : 해당 객체에 MonsterAttackComponent가 존재하지 않음");
			return;
		}


		// 이동방향이 바뀌는 순간 공격하는 방향을 바꾸기 위해 잠시 공격을 풀음
		if (diffPos.x > 0 && direction == Direction::Left) actor->GetComponent<MonsterAttackComponent>()->SetIsAttack(false);
		if (diffPos.x < 0 && direction == Direction::Right) actor->GetComponent<MonsterAttackComponent>()->SetIsAttack(false);

		// 해당 객체가 공격 중이면 중지
		if (actor->GetComponent<MonsterAttackComponent>()->GetIsAttack()) return;

		// 이동 방향 및 이동 속도를 구함
		diffPos.y -= (targetSize.y / 2 - actor->GetComponent<TransformComponent>()->GetScale().y / 2);	// 추적 객체의 아래로 이동하게 만듬 (적용하지 않았을 경우 몬스터가 공중에 뜨는 현상이 생김)

		diffPos.x /= static_cast<float>(sqrt(pow(diffPos.x, 2) + pow(diffPos.y, 2)));					// 이동 방향을 구함
		diffPos.y /= static_cast<float>(sqrt(pow(diffPos.x, 2) + pow(diffPos.y, 2)));

		speedX = ((diffPos.x * speed * 60) / Settings::Get().GetWidth());								// 각 방향 마다 이동 속도를 구함
		speedY = ((diffPos.y * speed * 60) / Settings::Get().GetHeight());

		// 위치를 더할 값을 저장
		toPos = D3DXVECTOR2(speedX, speedY);							

		// 스프라이트 
		std::vector<std::string> spriteName = actor->GetComponent<AnimatorComponent>()->GetAnimationFrameNames(); // 모든 애니메이션의 프레임 이름을 가져옴

		if (diffPos.x > 0)
		{
			// 오른쪽으로 이동하는 스프라이트를 찾음
			auto iter = std::find_if(spriteName.begin(), spriteName.end(), FindRightMoveSprite);

			if (iter == spriteName.end())
			{
				assert(false && "Eror (TraceComponent3) : 해당 객체에 \"RightMove\" Frame Name이 없음");
				return;
			}

			// 해당 스프라이트를 입력
			if(actor->GetComponent<AnimatorComponent>()->GetNowKeyFrameName() != *iter) 
				actor->GetComponent<AnimatorComponent>()->SetCurrentAnimation(*iter);

			direction = Direction::Right;
		}

		else if (diffPos.x < 0)
		{
			// 왼쪽으로 이동하는 스프라이트를 찾음
			std::vector<std::string>::iterator iter = std::find_if(spriteName.begin(), spriteName.end(), FindLeftMoveSprite);

			if (iter == spriteName.end())
			{
				assert(false && "Eror (TraceComponent4) : 해당 객체에 \"LeftMove\" Frame Name이 없음");
				return;
			}

			// 해당 스프라이트를 입력
			if (actor->GetComponent<AnimatorComponent>()->GetNowKeyFrameName() != *iter)
				actor->GetComponent<AnimatorComponent>()->SetCurrentAnimation(*iter);

			direction = Direction::Left;
		}
	}

	// 만약 어떠한 충돌 물체와 충돌한 경우 이동하지 않음
	if (actor->HasCompoent<CollisionComponent>() && actor->GetComponent<CollisionComponent>()->IsCollision()) 
		return;

	MoveToX = transform->GetPosition().x + speedX;
	MoveToY = transform->GetPosition().y + speedY;

	moveTo = D3DXVECTOR3(MoveToX, MoveToY, transform->GetPosition().z);	// 이동할 위치 설정

	transform->SetPosition(moveTo);										// 해당 위치로 이동
}

void TraceComponent::Destroy()
{
}
