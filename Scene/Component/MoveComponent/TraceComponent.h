// TraceComponent.h
// 해당 객체를 찾아가는 파일

#pragma once
#include "../../Actor.h"
#include "../IComponent.h"

class TraceComponent : public IComponent
{
public:
	using IComponent::IComponent;

	TraceComponent() = default;
	~TraceComponent() = default;

	void SetTarget(std::weak_ptr<class Actor> target) { this->target = target; }	// 충돌 목표 설정
	std::weak_ptr<class Actor> GetTarget() { return target; }

	void SetSpeed(float speed) { this->speed = speed; }
	const float GetSpeed() const { return speed; }

	const Direction GetDirection() const { return this->direction; }
	const D3DXVECTOR2 GetToPos() const { return toPos; }

	// IComponent을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Destroy() override;

public:
	static bool FindRightMoveSprite(std::string& spriteName) { return (spriteName.substr(spriteName.size() - 9, 9) == "RightMove"); }
	static bool FindLeftMoveSprite(std::string& spriteName) { return (spriteName.substr(spriteName.size() - 8, 8) == "LeftMove"); }

private:
	std::weak_ptr<class Actor> target;		// 타겟 객체 변수

	D3DXVECTOR3 targetPos;					// 타겟 객체의 위치를 받는 변수
	D3DXVECTOR3 targetSize;					// 타겟 크기

	Direction direction = Direction::Left;	// 이동 방향
	D3DXVECTOR2 toPos;						// 이동할 추가적인 위치
	float speed = 0.0f;						// 이동 속도
};