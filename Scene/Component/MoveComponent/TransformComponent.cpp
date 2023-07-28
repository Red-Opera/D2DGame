#include "stdafx.h"
#include "TransformComponent.h"

TransformComponent::TransformComponent(class Context* const context, Actor* actor, TransformComponent* transform) : IComponent(context, actor, transform)
{
	D3DXMatrixIdentity(&local);
	D3DXMatrixIdentity(&world);
	UpdateTransform();
}

TransformComponent::~TransformComponent()
{
}

void TransformComponent::Initialize()
{
}

void TransformComponent::Update()
{
	UpdateTransform();
}

void TransformComponent::Destroy()
{
}

void TransformComponent::SetLocalScale(const D3DXVECTOR3& scale)
{
	if (this->local_scale == scale) return;	// 입력한 크기가 현재 크기라면 중지

	// 크기행렬을 입력하고 UpdateTransform()함수를 호출하여 객체 world행렬 변경
	this->local_scale = scale;
	UpdateTransform();
}

void TransformComponent::SetLocalPosition(const D3DXVECTOR3& position)
{
	if (this->local_position == position) return;	// 입력한 위치가 현재 위치라면 중지

	// 위치행렬을 입력하고 UpdateTransform()함수를 호출하여 객체 world행렬 변경
	this->local_position = position;
	UpdateTransform();
}

void TransformComponent::SetLocalRotation(const D3DXVECTOR3& rotation)
{
	if (this->local_rotation == rotation) return;	// 입력한 회전이 현재 회전라면 중지

	// 회전행렬을 입력하고 UpdateTransform()함수를 호출하여 객체 world행렬 변경
	this->local_rotation = rotation;
	UpdateTransform();
}

const D3DXVECTOR3 TransformComponent::GetScale()
{
	D3DXVECTOR3 world_scale;	// 크기
	D3DXQUATERNION temp1;		// 회전 (임시값) 
	D3DXVECTOR3 temp2;			// 위치 (임시값)

	// 과제 : 오일러각, 짐벌락, 퀘터니언 정리
	// D3DXMatrixDecompose : world행렬에서 크기, 회전, 위치를 반환해주는 함수
	D3DXMatrixDecompose(&world_scale, &temp1, &temp2, &world);

	return world_scale;
}

void TransformComponent::SetScale(const D3DXVECTOR3& world_scale)
{
	if (GetScale() == world_scale) return;

	if (HasParent() == true)
	{
		D3DXVECTOR3 scale;
		D3DXVECTOR3 parent_scale = parent.lock()->GetScale();

		// World에서의 고유 객체 크기 = world에서 크기 / 부모 크기
		scale.x = world_scale.x / parent_scale.x;
		scale.y = world_scale.y / parent_scale.y;
		scale.z = world_scale.z / parent_scale.z;

		SetLocalScale(scale);
	}

	else
		SetLocalScale(world_scale);
}

const D3DXVECTOR3 TransformComponent::GetPosition()
{
	D3DXVECTOR3 temp1;			// 크기 (임시값)
	D3DXQUATERNION temp2;		// 회전 (임시값) 
	D3DXVECTOR3 world_position;	// 위치

	D3DXMatrixDecompose(&temp1, &temp2, &world_position, &world);

	return world_position;
}

void TransformComponent::SetPosition(const D3DXVECTOR3& world_position)
{
	if (GetPosition() == world_position) return;

	if (HasParent())
	{
		// 부모의 World 역행렬을 구한다.
		D3DXMATRIX inverse;
		D3DXMatrixInverse(&inverse, nullptr, &parent.lock()->GetWorldMatrix());

		// 부모의 역행렬을 곱해서 자식의 world행렬을 구하여 자식 world의 위치를 가져온다.
		D3DXVECTOR3 position;
		// D3DXVec3TransformCoord : 동차 값을 1로 지정하여 행렬 크기를 맞춰준다. (1X3 * 4X4 -> X,Y,X,1 * 4X4) 
		D3DXVec3TransformCoord(&position, &world_position, &inverse);

		SetLocalPosition(position);
	}

	else
		SetLocalPosition(world_position);
}

const D3DXVECTOR3 TransformComponent::GetRotation()
{
	D3DXVECTOR3 temp1;				// 크기 (임시값)
	D3DXQUATERNION world_rotation;	// 회전
	D3DXVECTOR3 temp2;				// 위치 (임시값, 해당 변수는 생략 가능 -> 필요없는 값이기 때문) 

	D3DXMatrixDecompose(&temp1, &world_rotation, &temp2, &world);

	D3DXMATRIX rotation;
	D3DXMatrixRotationQuaternion(&rotation, &world_rotation); // D3DXMatrixRotationQuaternion : D3DXQUATERNION -> D3DXMATRIX로 변환해주는 함수

	return D3DXVECTOR3 // Y - X - Z : 각각의 각을 구할 수 있는 식
	(
		static_cast<float>(atan2(rotation._31, rotation._33)),
		static_cast<float>(atan2(-rotation._32, sqrt(pow(rotation._12, 2) + pow(rotation._22, 2)))),
		static_cast<float>(atan2(rotation._12, rotation._22))
	);
}

void TransformComponent::SetRotation(const D3DXVECTOR3& world_rotation)
{
	if (GetRotation() == world_rotation) return;

	if (HasParent() == true)
	{
		D3DXMATRIX inverse;
		auto temp = GetWorldRotationMatrix();
		D3DXMatrixInverse(&inverse, nullptr, &temp);

		D3DXVECTOR3 rotation;
		D3DXVec3TransformNormal(&rotation, &world_rotation, &inverse); // D3DXVec3TransformNormal : 동차를 0으로 만들고 행렬을 D3DXVECTOR3로 만들어 주는 함수

		SetLocalRotation(rotation);
	}

	else
		SetLocalRotation(world_rotation);
}

const D3DXMATRIX TransformComponent::GetWorldRotationMatrix()
{
	D3DXQUATERNION world_rotation;
	D3DXVECTOR3 temp;

	// World행렬에서 World 회전 값을 가져오고
	D3DXMatrixDecompose(&temp, &world_rotation, &temp, &world);

	// World 회전 행렬로 변환한다.
	D3DXMATRIX rotation;
	D3DXMatrixRotationQuaternion(&rotation, &world_rotation);

	return rotation;
}

const D3DXVECTOR3 TransformComponent::GetRight() const
{
	D3DXVECTOR3 right;
	D3DXVECTOR3 vec(1, 0, 0);						// x방향

	D3DXVec3TransformNormal(&right, &vec, &world);	// world행렬과 곱해서 오른쪽 방향을 얻음

	return right;
}

const D3DXVECTOR3 TransformComponent::GetUp() const
{
	D3DXVECTOR3 up;
	D3DXVECTOR3 vec(0, 1, 0);					// y방향

	D3DXVec3TransformNormal(&up, &vec, &world);	// world행렬과 곱해서 위쪽 방향을 얻음

	return up;
}

const D3DXVECTOR3 TransformComponent::GetForward() const
{
	D3DXVECTOR3 forward;
	D3DXVECTOR3 vec(0, 0, 1);							// z방향

	D3DXVec3TransformNormal(&forward, &vec, &world);	// world행렬과 곱해서 앞쪽 방향을 얻음

	return forward;
}

void TransformComponent::SetParent(std::weak_ptr<TransformComponent> const new_parent)
{
	if (new_parent.expired() == true) return;
	if (new_parent.lock().get() == this) return;

	parent = new_parent;
}

std::weak_ptr<TransformComponent> const TransformComponent::GetChildFromIndex(const uint& index)
{
	std::shared_ptr<TransformComponent> temp;

	if (HasChilds() == false) return temp;
	if (index >= GetChildCount()) return temp;

	return childs[index];
}

void TransformComponent::AddChild(std::weak_ptr<TransformComponent> const child)
{
	if (child.expired() == true) return;
	if (child.lock().get() == this) return;

	childs.emplace_back(child);
}

void TransformComponent::UpdateTransform()
{
	// =============================================
	// [Local 생성]
	// =============================================

	D3DXMATRIX S, R, T;

	D3DXMatrixScaling(&S, local_scale.x, local_scale.y, local_scale.z);
	// Yaw : 비행기의 나침반이 바뀌는 방향으로 회전 축 (수직 방향 회전축), Pitch : 비행기의 고도가 상승하강하기 위한 회전 축 (횡(좌우) 방향 회전축), Roll : 비행기의 위아래가 바뀔수 있는 회전 축 (종(앞뒤) 방향 회전 축)
	D3DXMatrixRotationYawPitchRoll(&R, local_rotation.y /*Yaw*/, local_rotation.x/*Pitch*/, local_rotation.z/*Roll*/);
	D3DXMatrixTranslation(&T, local_position.x, local_position.y, local_position.z);

	local = S * R * T;

	// =============================================
	// [World로 변환]
	// =============================================

	// 부모 행렬로 연결
	if (HasParent() == true)
		world = local * parent.lock()->GetWorldMatrix();

	else
		world = local;

	// 해당 객체를 부모로 하는 자식을 모두 업데이트 함 (부모가 이동한 만큼 자식도 이동해야 하기 때문에)
	for (const auto& child : childs)
	{
		if (child.expired() != true)	// 자식이 있을 경우
			child.lock()->UpdateTransform();
	}
}
