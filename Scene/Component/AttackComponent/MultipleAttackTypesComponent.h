// MultipleAttackTypesComponent.h
// ��������Ʈ ������ ��Ÿ����� �ܰŸ����� �������ִ� ����

#pragma once
#include "../IComponent.h"

// �ܰŸ�, ��Ÿ� ����
enum class AttackType
{
	Short, Long
};

class MultipleAttackTypesComponent : public IComponent
{
public:
	using IComponent::IComponent;
	~MultipleAttackTypesComponent() = default;

	// IComponent��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Destroy() override;

	void SetAttackType(AttackType attackType) { this->attackType = attackType; }
	const AttackType GetAttackType() const { return attackType; }

private:
	AttackType attackType = AttackType::Short;
};