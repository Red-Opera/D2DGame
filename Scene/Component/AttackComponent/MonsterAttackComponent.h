#pragma once
#include "../IComponent.h"

/*
	Component ����
		1. TraceComponent�� ���� �߰��Ѵ�.
		2. ������ ��ü�� �ش� ���۳�Ʈ�� �߰��Ѵ�.
		3. ���ݷ°� ���ݰŸ��� �߰��Ѵ�.
*/

enum class ResizeTo { X, Y };

class MonsterAttackComponent : public IComponent
{
public:
	using IComponent::IComponent;

	// IComponent��(��) ���� ��ӵ�
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
	float delay;					// ��� ������ (���� : ��)
	float attackRange;				// ��ü�� ��Ÿ�
	float damage;					// ��ü�� ���ݷ�
		
	bool isAttack = false;			// �����ϴ��� ����

	ResizeTo axis = ResizeTo::X;	// Ȯ��� ��
	D3DXVECTOR3 multipleSpriteSize;	// ���ݽ� Ȯ��� ũ��
	bool isMulSize;					// Ȯ����� ����
	bool nowMulSize;				// ���� Ȯ��Ǿ� �ִ��� ����
};

