// TransformComponent.h
// Actor의 위치를 관리하기 위한 파일 (기능 : Local, World, Transform의 값을 읽고 쓰고 GPU버퍼(Constant Buffer) 제작, 부모 및 자식 객체가 있는지 확인)

#pragma once
#include "../IComponent.h"

class TransformComponent final : public IComponent
{
public:
	TransformComponent(class Context* const context, class Actor* actor, class TransformComponent* transform);
	~TransformComponent();

	// IComponent을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Destroy() override;

	// =============================================
	// [Local]
	// =============================================

	// 크기
	const D3DXVECTOR3& GetLocalScale() const { return local_scale; }
	void SetLocalScale(const D3DXVECTOR3& scale);

	// 위치
	const D3DXVECTOR3& GetLocalPosition() const { return local_position; }
	void SetLocalPosition(const D3DXVECTOR3& position);

	// 화전
	const D3DXVECTOR3& GetLocalRotation() const { return local_rotation; }
	void SetLocalRotation(const D3DXVECTOR3& rotation);

	// 로컬 위치
	const D3DXMATRIX& GetLocalMatrix() const { return local; }

	// =============================================
	// [World]
	// =============================================

	// World에서의 객체 고유 크기
	const D3DXVECTOR3 GetScale();
	void SetScale(const D3DXVECTOR3& world_scale);

	// World에서의 객체 고유 위치
	const D3DXVECTOR3 GetPosition();
	void SetPosition(const D3DXVECTOR3& world_position);

	// World에서의 객체 고유 회전
	const D3DXVECTOR3 GetRotation();
	void SetRotation(const D3DXVECTOR3& world_rotation);

	const D3DXMATRIX GetWorldRotationMatrix();	// 내부에서 사용하기 위한 회전행렬 반환 함수

	const D3DXMATRIX& GetWorldMatrix() const { return world; }

	// 캐릭터 기준으로의 방향을 반환하는 함수
	const D3DXVECTOR3 GetRight() const;
	const D3DXVECTOR3 GetUp() const;
	const D3DXVECTOR3 GetForward() const;

	// =============================================
	// [Transform]
	// =============================================

	void UpdateTransform();						// S, R, T값을 하나로 합쳐 local행렬 만든 후 world행렬로 만드는 함수
	bool HasParent() const { return parent.expired() ? false : true; } // 스마트 포인터는 변수 이름만으로도 bool값을 구할 수 없어 따로 값을 처리해서 구해야 한다.

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
	// 자신의 S, R, T 값
	D3DXVECTOR3 local_scale = D3DXVECTOR3(1, 1, 1);
	D3DXVECTOR3 local_position = D3DXVECTOR3(0, 0, 0);
	D3DXVECTOR3 local_rotation = D3DXVECTOR3(0, 0, 0);

	// world = S * R * T * r * P;
	D3DXMATRIX local;	// local = S * R * T;
	D3DXMATRIX world;	// local * parent;

	std::weak_ptr<TransformComponent> parent;				// 부모 객체 데이터가 없어지면 자식 포인터도 만료되기 위해서 weak_ptr로 받음
	std::vector<std::weak_ptr<TransformComponent>> childs;	// 해당 객체의 부품

	D3DXVECTOR3 front = D3DXVECTOR3();						// 앞방향
};