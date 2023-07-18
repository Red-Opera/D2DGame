#pragma once
#include "../IComponent.h"

class PlayerSkillComponent : public IComponent
{
public:
	using IComponent::IComponent;
	~PlayerSkillComponent() = default;

	// IComponent��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Destroy() override;

	void SetManaComsum(unsigned int mana) { this->manaConsum = mana; }
	const unsigned int GetManaComsum() const { return manaConsum; }

private:
	 class std::shared_ptr<class Actor> gaugeBar;		// �÷��̾� ������ ��

	unsigned int manaConsum = 0;						// ��ų�� �������� ����ϴ� ����

	float delay = 0.5f;											// ��� ������ (���� : ��)
	float nowDelay = 0.5f;										// ���� ������

	Timer* timer;												// Ÿ�Ӹ�
};

