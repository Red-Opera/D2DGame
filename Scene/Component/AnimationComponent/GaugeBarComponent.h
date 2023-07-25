// GaugeBarComponent.h
// 게이지 바(체력 및 마나) 및 객체의 체력와 마나 관련 파일

/*
	Component 사용법
		1. 게이지 바를 객체를 Scene 생성자에 생성한다.
		2. 게이지 바에 Scene과 TransformComponent, GaugeBarComponent을 등록한다.
		3. 게이지 바 객체에 SetBasicState 메소드를 이용하여 게이지 바의 기본정보를 입력한다.
		4. 게이지 바 객체에 SetBarTransform 메소드를 이용하여 각각의 위치를 입력한다.
		5. 게이지 바 객체와 게이지 바를 붙일 객체를 각각 자식과 부모로 연결한다.
		6. 게이지 바 전체적인 크기를 정한다.
		7. 게이지 바 이름을 정한다.
*/

#pragma once
#include "../../Actor.h"
#include "../IComponent.h"

// 체력 및 마나 바의 위치와 크기
struct BarTransform
{
	// 위치 (단위 픽셀)
	D3DXVECTOR2 hpBarPos = D3DXVECTOR2(0.0f, 0.0f);			// HP바 위치
	D3DXVECTOR2 manaBarPos = D3DXVECTOR2(0.0f, 0.0f);		// Mana바 위치

	// 사이즈 (단위 : 픽셀)
	D3DXVECTOR2 hpBarSize = D3DXVECTOR2(0.0f, 0.0f);		// HP바 크기
	D3DXVECTOR2 manaBarSize = D3DXVECTOR2(0.0f, 0.0f);		// Mana바 크기
};

struct GaugeScalePos
{
	D3DXVECTOR2 hpBarGaugePos = D3DXVECTOR2(0.0f, 0.0f);	// HP바 게이지 위치
	D3DXVECTOR2 manaBarGaugePos = D3DXVECTOR2(0.0f, 0.0f);	// Mana바 게이지 위치

	D3DXVECTOR2 hpBarGaugeSize = D3DXVECTOR2(0.0f, 0.0f);	// HP바 게이지 크기
	D3DXVECTOR2 manaBarGaugeSize = D3DXVECTOR2(0.0f, 0.0f);	// Mana게이지 크기
};

class GaugeBarComponent : public IComponent
{
public:
	using IComponent::IComponent;
	~GaugeBarComponent() = default;

	// IComponent을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Destroy() override;

	void SetBasicState(float HP, float mana) { this->maxHP = HP, this->maxMana = mana; hp = maxHP; this->mana = maxMana; }
	void GetBasicState() { std::cout << this->GetActor()->GetName() << "의 체력 : " << this->hp << ", 마나 : " << this->mana; }

	void SetBarTransform(const BarTransform& barTransform, const float hpPersent = 1.0f, const float manaPersent = 1.0f);
	BarTransform& GetBarTransform() { return stateBarTransform; }

	void SetDamegeHp(float hp) { this->hp -= hp; }

	void SetMana(float mana) { this->mana = mana; }
	void AddMana(float mana) { this->mana += mana; }

	float GetMaxHp() { return maxHP; }
	float GetMaxMana() { return maxMana; }

	float GetHp() { return hp; }
	float GetMana() { return mana; }

	float GetHpPersent() { return hp / maxHP; }
	float GetManaPersent() { return mana / maxMana; }

	bool GetIsDead() { return isDead; }

private:
	class Timer* timer = nullptr;	// 프레임 시간 측정을 위한 변수

	// 상태 바 위치
	BarTransform stateBarTransform;
	GaugeScalePos gaugeScalePos;

	// 상태 바의 구성 (체력 바, 마나 바, 체력 게이지, 마나 게이지)
	std::map<std::string, std::shared_ptr<Actor>> children;

	// 최대 상태
	float maxHP = 0;
	float maxMana = 0;

	// 현재 상태
	float hp = 0;
	float mana = 0;
	bool isDead = false;
};

