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
	collisionObject.clear();	// �浹�� ��ü �ʱ�ȭ

	int i = 0;

	for (auto& tar : target)
	{
		if (tar.expired()) continue;

		// �浹 ��ü�� ���� ��ü�� ������ ������
		targetPos = tar.lock()->GetTransform()->GetPosition();
		targetSize = tar.lock()->GetTransform()->GetScale();
		StartArea targetArea = area[i];
		float targetLimit = limitPersent[i++];

		// ����ڰ� �Է��� �浹 ó�� ������ targetSize�� �Է���
		switch (targetArea)
		{
		case StartArea::Top:
			targetPos.y += targetSize.y / 2 - targetSize.y * targetLimit / 2;		// �߽���ġ -> �浹 ��ü �� -> ���� ��ġ�� �غ����� �̵�
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

		// �� ���۳�Ʈ�� ���� ��ü�� Transform ����
		AbleArea thisArea = { transform->GetPosition(), transform->GetScale() };

		// �̵��ϴ� �ش� ��ü�� �÷��̾��� ���
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

		// �̵��ϴ� �ش� ��ü�� �÷��̾ ������� ��ü�� ���
		else if (actor->HasCompoent<TraceComponent>())
		{
			thisArea.pos.x += actor->GetComponent<TraceComponent>()->GetToPos().x;
			thisArea.pos.y += actor->GetComponent<TraceComponent>()->GetToPos().y;
		}

		isCollision = Intersect::IsIntersect(ableArea, thisArea);	// �ش� ��ü(ableArea)�� �� ���۳�Ʈ�� ���� ��ü�� �浹�ߴ��� �Ǵ�

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