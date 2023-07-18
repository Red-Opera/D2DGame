// CollisionComponent.h
// �ٸ� ��ü�� �浹�ߴ��� Ȯ���ϴ� ����

#pragma once
#include "../IComponent.h"

// �浹 ó�� ���� ���� Enum
enum class StartArea
{
	Top,
	Left,
	Right,
	Bottom
};

struct AbleArea { D3DXVECTOR3 pos, size; };

class CollisionComponent : public IComponent
{
public:
	IComponent::IComponent;
	~CollisionComponent() = default;

	// IComponent��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Destroy() override;
		
	void SetCollistionTarget(std::weak_ptr<class Actor> target) { this->target.push_back(target); }
	void SetCollisionArea(StartArea area, float size);
	
	const std::list<std::weak_ptr<class Actor>>& GetCollisionObject() const { return collisionObject; }
 	AbleArea& GetCollistionArea() { return ableArea; }

	bool IsCollision() { return isCollision; };

private:
	// �浹 üũ�� ��ü
	std::list<std::weak_ptr<class Actor>> target;			// Ÿ�� ��ü ����
	std::list<std::weak_ptr<class Actor>> collisionObject;	// �浹�� ��ü

	D3DXVECTOR3 targetPos;									// Ÿ�� ��ü�� ��ġ�� �޴� ����
	D3DXVECTOR3 targetSize;									// Ÿ�� ũ��

	// �ٸ� ��ü���� �浹 ������ ���ġ
	AbleArea ableArea;		// ���� �浹 ���� ����

	std::vector<StartArea> area;							// ���� ��ġ
	std::vector<float> limitPersent;						// �浹 ���ġ (�ۼ�Ʈ)

	// �浹 ����
	bool isCollision;						// �浹 �ߴ��� ����
};

