#include <algorithm>

#include "stdafx.h"
#include "CollisionComponent.h"
#include "TransformComponent.h"
#include "MoveScriptComponent.h"
#include "TraceComponent.h"
#include "Scene/Scene.h"

#include "../../Actor.h"

#include "Intersect/Intersect.h"

void CollisionComponent::Initialize()
{
	ableArea.pos = D3DXVECTOR3();
	ableArea.size = D3DXVECTOR3();
}

void CollisionComponent::Update()
{
	collisionObject.clear();	// 충돌한 객체 초기화

	int i = 0;

	for (auto& tar : target)
	{
		if (tar.expired()) continue;

		// 충돌 객체로 만들 객체의 정보를 가져옴
		targetPos = tar.lock()->GetTransform()->GetPosition();
		targetSize = tar.lock()->GetTransform()->GetScale();
		StartArea targetArea = area[i];
		float targetLimit = limitPersent[i++];

		// 사용자가 입력한 충돌 처리 영역을 targetSize에 입력함
		switch (targetArea)
		{
		case StartArea::Top:
			targetPos.y += targetSize.y / 2 - targetSize.y * targetLimit / 2;		// 중심위치 -> 충돌 객체 위 -> 위변 위치를 밑변으로 이동
			targetSize.y *= targetLimit;
			break;

		case StartArea::Left:
			targetPos.x += targetSize.x / 2 - targetSize.x * targetLimit / 2;
			targetSize.x *= targetLimit;
			break;

		case StartArea::Right:
			targetPos.x -= targetSize.x / 2 - targetSize.x * targetLimit / 2;
			targetSize.x *= targetLimit;
			break;

		case StartArea::Bottom:
			targetPos.y -= targetSize.y / 2 - targetSize.y * targetLimit / 2;
			targetSize.y *= targetLimit;
			break;

		default:
			break;
		}

		ableArea.pos = targetPos;
		ableArea.size = targetSize;

		// 이 컴퍼넌트를 넣은 객체의 Transform 설정
		AbleArea thisArea = { transform->GetPosition(), transform->GetScale() };

		// 이동하는 해당 객체가 플레이어일 경우
		if (actor->GetName() == "Player")
		{
			switch (actor->GetDirection())
			{
			case Direction::Front:
				thisArea.pos.y += actor->GetComponent<MoveScriptComponent>()->GetMoveSpeed();
				break;

			case Direction::Back:
				thisArea.pos.y -= actor->GetComponent<MoveScriptComponent>()->GetMoveSpeed();
				break;

			case Direction::Left:
				thisArea.pos.x -= actor->GetComponent<MoveScriptComponent>()->GetMoveSpeed();
				break;

			case Direction::Right:
				thisArea.pos.x += actor->GetComponent<MoveScriptComponent>()->GetMoveSpeed();
				break;
			}
		}

		// 이동하는 해당 객체가 플레이어를 따라오는 객체일 경우
		else if (actor->HasCompoent<TraceComponent>())
		{
			thisArea.pos.x += actor->GetComponent<TraceComponent>()->GetToPos().x;
			thisArea.pos.y += actor->GetComponent<TraceComponent>()->GetToPos().y;
		}

		isCollision = Intersect::IsIntersect(ableArea, thisArea);	// 해당 객체(ableArea)와 이 컴퍼넌트를 넣은 객체가 충돌했는지 판단

	    if (isCollision)
	    {
	    	collisionObject.emplace_back(tar);
	    
	      //if (ImGui::Begin("Collision"))
	      //{
	      //	ImGui::InputFloat3((tar.lock()->GetName() + "Pos").c_str(), ableArea.pos);
	      //	ImGui::InputFloat3((tar.lock()->GetName() + "Size").c_str(), ableArea.size);
	      //}
	      //ImGui::End();
	      //
	      //if (ImGui::Begin("Collision"))
	      //{
	      //	ImGui::InputFloat3((actor->GetName() + "Pos").c_str(), thisArea.pos);
	      //	ImGui::InputFloat3((actor->GetName() + "Size").c_str(), thisArea.size);
	      //}
	      //ImGui::End();
	    }
	}

	if (collisionObject.empty()) isCollision = false;
	else isCollision = true;
}

void CollisionComponent::Destroy()
{

}

void CollisionComponent::SetCollisionArea(StartArea area, float size)
{
	this->area.push_back(area);
	this->limitPersent.push_back(size);
}