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
	if (this->local_scale == scale) return;	// �Է��� ũ�Ⱑ ���� ũ���� ����

	// ũ������� �Է��ϰ� UpdateTransform()�Լ��� ȣ���Ͽ� ��ü world��� ����
	this->local_scale = scale;
	UpdateTransform();
}

void TransformComponent::SetLocalPosition(const D3DXVECTOR3& position)
{
	if (this->local_position == position) return;	// �Է��� ��ġ�� ���� ��ġ��� ����

	// ��ġ����� �Է��ϰ� UpdateTransform()�Լ��� ȣ���Ͽ� ��ü world��� ����
	this->local_position = position;
	UpdateTransform();
}

void TransformComponent::SetLocalRotation(const D3DXVECTOR3& rotation)
{
	if (this->local_rotation == rotation) return;	// �Է��� ȸ���� ���� ȸ����� ����

	// ȸ������� �Է��ϰ� UpdateTransform()�Լ��� ȣ���Ͽ� ��ü world��� ����
	this->local_rotation = rotation;
	UpdateTransform();
}

const D3DXVECTOR3 TransformComponent::GetScale()
{
	D3DXVECTOR3 world_scale;	// ũ��
	D3DXQUATERNION temp1;		// ȸ�� (�ӽð�) 
	D3DXVECTOR3 temp2;			// ��ġ (�ӽð�)

	// ���� : ���Ϸ���, ������, ���ʹϾ� ����
	// D3DXMatrixDecompose : world��Ŀ��� ũ��, ȸ��, ��ġ�� ��ȯ���ִ� �Լ�
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

		// World������ ���� ��ü ũ�� = world���� ũ�� / �θ� ũ��
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
	D3DXVECTOR3 temp1;			// ũ�� (�ӽð�)
	D3DXQUATERNION temp2;		// ȸ�� (�ӽð�) 
	D3DXVECTOR3 world_position;	// ��ġ

	D3DXMatrixDecompose(&temp1, &temp2, &world_position, &world);

	return world_position;
}

void TransformComponent::SetPosition(const D3DXVECTOR3& world_position)
{
	if (GetPosition() == world_position) return;

	if (HasParent())
	{
		// �θ��� World ������� ���Ѵ�.
		D3DXMATRIX inverse;
		D3DXMatrixInverse(&inverse, nullptr, &parent.lock()->GetWorldMatrix());

		// �θ��� ������� ���ؼ� �ڽ��� world����� ���Ͽ� �ڽ� world�� ��ġ�� �����´�.
		D3DXVECTOR3 position;
		// D3DXVec3TransformCoord : ���� ���� 1�� �����Ͽ� ��� ũ�⸦ �����ش�. (1X3 * 4X4 -> X,Y,X,1 * 4X4) 
		D3DXVec3TransformCoord(&position, &world_position, &inverse);

		SetLocalPosition(position);
	}

	else
		SetLocalPosition(world_position);
}

const D3DXVECTOR3 TransformComponent::GetRotation()
{
	D3DXVECTOR3 temp1;				// ũ�� (�ӽð�)
	D3DXQUATERNION world_rotation;	// ȸ��
	D3DXVECTOR3 temp2;				// ��ġ (�ӽð�, �ش� ������ ���� ���� -> �ʿ���� ���̱� ����) 

	D3DXMatrixDecompose(&temp1, &world_rotation, &temp2, &world);

	D3DXMATRIX rotation;
	D3DXMatrixRotationQuaternion(&rotation, &world_rotation); // D3DXMatrixRotationQuaternion : D3DXQUATERNION -> D3DXMATRIX�� ��ȯ���ִ� �Լ�

	return D3DXVECTOR3 // Y - X - Z : ������ ���� ���� �� �ִ� ��
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
		D3DXVec3TransformNormal(&rotation, &world_rotation, &inverse); // D3DXVec3TransformNormal : ������ 0���� ����� ����� D3DXVECTOR3�� ����� �ִ� �Լ�

		SetLocalRotation(rotation);
	}

	else
		SetLocalRotation(world_rotation);
}

const D3DXMATRIX TransformComponent::GetWorldRotationMatrix()
{
	D3DXQUATERNION world_rotation;
	D3DXVECTOR3 temp;

	// World��Ŀ��� World ȸ�� ���� ��������
	D3DXMatrixDecompose(&temp, &world_rotation, &temp, &world);

	// World ȸ�� ��ķ� ��ȯ�Ѵ�.
	D3DXMATRIX rotation;
	D3DXMatrixRotationQuaternion(&rotation, &world_rotation);

	return rotation;
}

const D3DXVECTOR3 TransformComponent::GetRight() const
{
	D3DXVECTOR3 right;
	D3DXVECTOR3 vec(1, 0, 0);						// x����

	D3DXVec3TransformNormal(&right, &vec, &world);	// world��İ� ���ؼ� ������ ������ ����

	return right;
}

const D3DXVECTOR3 TransformComponent::GetUp() const
{
	D3DXVECTOR3 up;
	D3DXVECTOR3 vec(0, 1, 0);					// y����

	D3DXVec3TransformNormal(&up, &vec, &world);	// world��İ� ���ؼ� ���� ������ ����

	return up;
}

const D3DXVECTOR3 TransformComponent::GetForward() const
{
	D3DXVECTOR3 forward;
	D3DXVECTOR3 vec(0, 0, 1);							// z����

	D3DXVec3TransformNormal(&forward, &vec, &world);	// world��İ� ���ؼ� ���� ������ ����

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
	// [Local ����]
	// =============================================

	D3DXMATRIX S, R, T;

	D3DXMatrixScaling(&S, local_scale.x, local_scale.y, local_scale.z);
	// Yaw : ������� ��ħ���� �ٲ�� �������� ȸ�� �� (���� ���� ȸ����), Pitch : ������� ���� ����ϰ��ϱ� ���� ȸ�� �� (Ⱦ(�¿�) ���� ȸ����), Roll : ������� ���Ʒ��� �ٲ�� �ִ� ȸ�� �� (��(�յ�) ���� ȸ�� ��)
	D3DXMatrixRotationYawPitchRoll(&R, local_rotation.y /*Yaw*/, local_rotation.x/*Pitch*/, local_rotation.z/*Roll*/);
	D3DXMatrixTranslation(&T, local_position.x, local_position.y, local_position.z);

	local = S * R * T;

	// =============================================
	// [World�� ��ȯ]
	// =============================================

	// �θ� ��ķ� ����
	if (HasParent() == true)
		world = local * parent.lock()->GetWorldMatrix();

	else
		world = local;

	// �ش� ��ü�� �θ�� �ϴ� �ڽ��� ��� ������Ʈ �� (�θ� �̵��� ��ŭ �ڽĵ� �̵��ؾ� �ϱ� ������)
	for (const auto& child : childs)
	{
		if (child.expired() != true)	// �ڽ��� ���� ���
			child.lock()->UpdateTransform();
	}
}
