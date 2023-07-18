#pragma once
#include "../IComponent.h"

class PlayerSkillComponent : public IComponent
{
public:
	using IComponent::IComponent;
	~PlayerSkillComponent() = default;

	// IComponent을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Destroy() override;

	void SetManaComsum(unsigned int mana) { this->manaConsum = mana; }
	const unsigned int GetManaComsum() const { return manaConsum; }

private:
	 class std::shared_ptr<class Actor> gaugeBar;		// 플레이어 게이지 바

	unsigned int manaConsum = 0;						// 스킬을 쓸때마다 사용하는 마나

	float delay = 0.5f;											// 사격 딜레이 (단위 : 초)
	float nowDelay = 0.5f;										// 남은 딜레이

	Timer* timer;												// 타임머
};

