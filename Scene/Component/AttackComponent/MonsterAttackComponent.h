#pragma once
#include "../IComponent.h"

/*
	Component 사용법
		1. TraceComponent를 먼저 추가한다.
		2. 공격할 객체를 해당 컴퍼넌트를 추가한다.
		3. 공격력과 공격거리를 추가한다.
*/

enum class ResizeTo { X, Y };

class MonsterAttackComponent : public IComponent
{
public:
	using IComponent::IComponent;

	// IComponent을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Destroy() override;

	void SetAttackDelay(const float delay) { this->delay = delay; }
	float GetAttackDelay() { return delay; }

	void SetAttackRange(const float attackRange) { this->attackRange = attackRange; }
	const float GetAttackRange() const { return attackRange; }

	void SetDamage(const float damage) { this->damage = damage; }
	const float GetDamage() const { return damage; }

	void SetIsAttack(const bool isAttack) { this->isAttack = isAttack; }
	bool GetIsAttack() { return isAttack; }

	void SetSizeChange(float mul, ResizeTo axis) { this->multipleSpriteSize *= mul; this->axis = axis; this->isMulSize; }

private:
	float delay;					// 사격 딜레이 (단위 : 초)
	float attackRange;				// 객체의 사거리
	float damage;					// 객체의 공격력
		
	bool isAttack = false;			// 공격하는지 구분

	ResizeTo axis = ResizeTo::X;	// 확대될 축
	D3DXVECTOR3 multipleSpriteSize;	// 공격시 확대될 크기
	bool isMulSize;					// 확대될지 여부
	bool nowMulSize;				// 현재 확대되어 있는지 여부
};

