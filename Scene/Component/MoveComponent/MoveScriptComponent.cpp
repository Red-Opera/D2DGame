#include "stdafx.h"
#include "MoveScriptComponent.h"
#include "TransformComponent.h"
#include "CollisionComponent.h"
#include "../MeshRendererComponent.h"
#include "../AnimationComponent/AnimatorComponent.h"
#include "../../Actor.h"

#define HOLDPRESSKEYBOARD(input, key) (input->GetKeyState(key) == KEY_HOLD || input->GetKeyState(key) == KEY_DOWN)
#define ONLYPRESSKEYBOARD(input, key) (input->GetKeyState(key) == KEY_DOWN)

#define HOLDPRESSMOUSE(input, key) (input->GetMouseState(key) == KEY_HOLD || input->GetMouseState(key) == KEY_DOWN)
#define ONLYPRESSMOUSE(input, key) (input->GetMouseState(key) == KEY_DOWN)

void MoveScriptComponent::Initialize()
{
	
}

void MoveScriptComponent::Update()
{
	D3DXVECTOR3 position = transform->GetPosition();

	auto input = context->GetSubSystem<InputManager>();
	auto animation = GetActor()->GetComponent<AnimatorComponent>();

	if (HOLDPRESSKEYBOARD(input, DIK_W))
	{
		GetActor()->SetDirection(Direction::Front);

		if (position.y + speed > (WorldY / 2))
			return;

		// 현재 애니메이션이 같은 방향의 이동 애니메이션이 아닌 경우 해당 이동 방향의 애니메이션 실행
		if(animation->GetNowKeyFrameName() != "FrontMove") animation->SetCurrentAnimation("FrontMove");
		if(!actor->GetComponent<CollisionComponent>()->IsCollision()) position.y += speed;
	}

	else if (HOLDPRESSKEYBOARD(input, DIK_S))
	{
		GetActor()->SetDirection(Direction::Back);

		if (position.y - speed < (-WorldY / 2))
			return;

		if (animation->GetNowKeyFrameName() != "BackMove") animation->SetCurrentAnimation("BackMove");
		if (!actor->GetComponent<CollisionComponent>()->IsCollision()) position.y -= speed;
	}

	else if (HOLDPRESSKEYBOARD(input, DIK_A))
	{
		GetActor()->SetDirection(Direction::Left);

		if (position.x - speed < (-WorldX / 2))
			return;

		if (animation->GetNowKeyFrameName() != "LeftMove") animation->SetCurrentAnimation("LeftMove");
		if (!actor->GetComponent<CollisionComponent>()->IsCollision()) position.x -= speed;
	}

	else if (HOLDPRESSKEYBOARD(input, DIK_D))
	{
		GetActor()->SetDirection(Direction::Right);

		if (position.x + speed > (WorldX / 2))
			return;

		if (animation->GetNowKeyFrameName() != "RightMove") animation->SetCurrentAnimation("RightMove");
		if (!actor->GetComponent<CollisionComponent>()->IsCollision()) position.x += speed;
	}
	
	else
	{
		switch (GetActor()->GetDirection())
		{
		case Direction::Front:
			if (animation->GetNowKeyFrameName() != "FrontIdle") animation->SetCurrentAnimation("FrontIdle"); 

			break;

		case Direction::Back:
			if (animation->GetNowKeyFrameName() != "BackIdle") animation->SetCurrentAnimation("BackIdle");
			break;

		case Direction::Left:
			if (animation->GetNowKeyFrameName() != "LeftIdle") animation->SetCurrentAnimation("LeftIdle");
			break;

		case Direction::Right:
			if (animation->GetNowKeyFrameName() != "RightIdle") animation->SetCurrentAnimation("RightIdle");
			break;

		default:
			break;
		}
	}

	transform->SetPosition(position);

	//std::cout << "(" << ImGui::GetIO(). << ", " << ImGui::GetWindowPos().y << ")";

	// 프레임 관련 정보
	//if(ImGui::Begin("FPS"))
	//	ImGui::Text(u8"Application 평균 프레임 : %.3f ms (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	//ImGui::End();
}

void MoveScriptComponent::Destroy()
{
}
