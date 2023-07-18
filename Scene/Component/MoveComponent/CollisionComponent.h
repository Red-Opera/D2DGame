// CollisionComponent.h
// 다른 객체와 충돌했는지 확인하는 파일

#pragma once
#include "../IComponent.h"

// 충돌 처리 시작 영역 Enum
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

	// IComponent을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Destroy() override;
		
	void SetCollistionTarget(std::weak_ptr<class Actor> target) { this->target.push_back(target); }
	void SetCollisionArea(StartArea area, float size);
	
	const std::list<std::weak_ptr<class Actor>>& GetCollisionObject() const { return collisionObject; }
 	AbleArea& GetCollistionArea() { return ableArea; }

	bool IsCollision() { return isCollision; };

private:
	// 충돌 체크할 객체
	std::list<std::weak_ptr<class Actor>> target;			// 타겟 객체 변수
	std::list<std::weak_ptr<class Actor>> collisionObject;	// 충돌한 객체

	D3DXVECTOR3 targetPos;									// 타겟 객체의 위치를 받는 변수
	D3DXVECTOR3 targetSize;									// 타겟 크기

	// 다른 객체와의 충돌 가능한 허용치
	AbleArea ableArea;		// 최종 충돌 가능 영역

	std::vector<StartArea> area;							// 시작 위치
	std::vector<float> limitPersent;						// 충돌 허용치 (퍼센트)

	// 충돌 여부
	bool isCollision;						// 충돌 했는지 여부
};

