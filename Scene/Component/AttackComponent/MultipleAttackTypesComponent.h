// MultipleAttackTypesComponent.h
// 스프라이트 공격이 장거리인지 단거리인지 설정해주는 파일

#pragma once
#include "../IComponent.h"

// 단거리, 장거리 구분
enum class AttackType
{
	Short, Long
};

class MultipleAttackTypesComponent : public IComponent
{
public:
	using IComponent::IComponent;
	~MultipleAttackTypesComponent() = default;

	// IComponent을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Destroy() override;

	void SetAttackType(AttackType attackType) { this->attackType = attackType; }
	const AttackType GetAttackType() const { return attackType; }

private:
	AttackType attackType = AttackType::Short;
};