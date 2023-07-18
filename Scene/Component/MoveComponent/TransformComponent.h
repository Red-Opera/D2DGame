// TransformComponent.h
// Actor�� ��ġ�� �����ϱ� ���� ���� (��� : Local, World, Transform�� ���� �а� ���� GPU����(Constant Buffer) ����, �θ� �� �ڽ� ��ü�� �ִ��� Ȯ��)

#pragma once
#include "../IComponent.h"

class TransformComponent final : public IComponent
{
public:
	TransformComponent(class Context* const context, class Actor* actor, class TransformComponent* transform);
	~TransformComponent();

	// IComponent��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Destroy() override;

	// =============================================
	// [Local]
	// =============================================

	// ũ��
	const D3DXVECTOR3& GetLocalScale() const { return local_scale; }
	void SetLocalScale(const D3DXVECTOR3& scale);

	// ��ġ
	const D3DXVECTOR3& GetLocalPosition() const { return local_position; }
	void SetLocalPosition(const D3DXVECTOR3& position);

	// ȭ��
	const D3DXVECTOR3& GetLocalRotation() const { return local_rotation; }
	void SetLocalRotation(const D3DXVECTOR3& rotation);

	// ���� ��ġ
	const D3DXMATRIX& GetLocalMatrix() const { return local; }

	// =============================================
	// [World]
	// =============================================

	// World������ ��ü ���� ũ��
	const D3DXVECTOR3 GetScale();
	void SetScale(const D3DXVECTOR3& world_scale);

	// World������ ��ü ���� ��ġ
	const D3DXVECTOR3 GetPosition();
	void SetPosition(const D3DXVECTOR3& world_position);

	// World������ ��ü ���� ȸ��
	const D3DXVECTOR3 GetRotation();
	void SetRotation(const D3DXVECTOR3& world_rotation);

	const D3DXMATRIX GetWorldRotationMatrix();	// ���ο��� ����ϱ� ���� ȸ����� ��ȯ �Լ�

	const D3DXMATRIX& GetWorldMatrix() const { return world; }

	// ĳ���� ���������� ������ ��ȯ�ϴ� �Լ�
	const D3DXVECTOR3 GetRight() const;
	const D3DXVECTOR3 GetUp() const;
	const D3DXVECTOR3 GetForward() const;

	// =============================================
	// [Transform]
	// =============================================

	void UpdateTransform();						// S, R, T���� �ϳ��� ���� local��� ���� �� world��ķ� ����� �Լ�
	bool HasParent() const { return parent.expired() ? false : true; } // ����Ʈ �����ʹ� ���� �̸������ε� bool���� ���� �� ���� ���� ���� ó���ؼ� ���ؾ� �Ѵ�.

	bool HasChilds() const { return childs.empty() != true; }

	std::weak_ptr<TransformComponent> const GetParent() { return parent; }
	void SetParent(std::weak_ptr<TransformComponent> const new_parent);

	const std::vector<std::weak_ptr<TransformComponent>>& GetChilds() const { return childs; }

	std::weak_ptr<TransformComponent> const GetChildFromIndex(const uint& index);

	uint GetChildCount() const { return static_cast<uint>(childs.size()); }

	void AddChild(std::weak_ptr<TransformComponent> const child);

	void SetFront(D3DXVECTOR3 front) { this->front = front; }
	const D3DXVECTOR3& GetFront() const { return front; }

private:
	// �ڽ��� S, R, T ��
	D3DXVECTOR3 local_scale = D3DXVECTOR3(1, 1, 1);
	D3DXVECTOR3 local_position = D3DXVECTOR3(0, 0, 0);
	D3DXVECTOR3 local_rotation = D3DXVECTOR3(0, 0, 0);

	// world = S * R * T * r * P;
	D3DXMATRIX local;	// local = S * R * T;
	D3DXMATRIX world;	// local * parent;

	std::weak_ptr<TransformComponent> parent;				// �θ� ��ü �����Ͱ� �������� �ڽ� �����͵� ����Ǳ� ���ؼ� weak_ptr�� ����
	std::vector<std::weak_ptr<TransformComponent>> childs;	// �ش� ��ü�� ��ǰ

	D3DXVECTOR3 front = D3DXVECTOR3();						// �չ���
};