// GaugeBarComponent.h
// ������ ��(ü�� �� ����) �� ��ü�� ü�¿� ���� ���� ����

/*
	Component ����
		1. ������ �ٸ� ��ü�� Scene �����ڿ� �����Ѵ�.
		2. ������ �ٿ� Scene�� TransformComponent, GaugeBarComponent�� ����Ѵ�.
		3. ������ �� ��ü�� SetBasicState �޼ҵ带 �̿��Ͽ� ������ ���� �⺻������ �Է��Ѵ�.
		4. ������ �� ��ü�� SetBarTransform �޼ҵ带 �̿��Ͽ� ������ ��ġ�� �Է��Ѵ�.
		5. ������ �� ��ü�� ������ �ٸ� ���� ��ü�� ���� �ڽİ� �θ�� �����Ѵ�.
		6. ������ �� ��ü���� ũ�⸦ ���Ѵ�.
		7. ������ �� �̸��� ���Ѵ�.
*/

#pragma once
#include "../../Actor.h"
#include "../IComponent.h"

// ü�� �� ���� ���� ��ġ�� ũ��
struct BarTransform
{
	// ��ġ (���� �ȼ�)
	D3DXVECTOR2 hpBarPos = D3DXVECTOR2(0.0f, 0.0f);			// HP�� ��ġ
	D3DXVECTOR2 manaBarPos = D3DXVECTOR2(0.0f, 0.0f);		// Mana�� ��ġ

	// ������ (���� : �ȼ�)
	D3DXVECTOR2 hpBarSize = D3DXVECTOR2(0.0f, 0.0f);		// HP�� ũ��
	D3DXVECTOR2 manaBarSize = D3DXVECTOR2(0.0f, 0.0f);		// Mana�� ũ��
};

struct GaugeScalePos
{
	D3DXVECTOR2 hpBarGaugePos = D3DXVECTOR2(0.0f, 0.0f);	// HP�� ������ ��ġ
	D3DXVECTOR2 manaBarGaugePos = D3DXVECTOR2(0.0f, 0.0f);	// Mana�� ������ ��ġ

	D3DXVECTOR2 hpBarGaugeSize = D3DXVECTOR2(0.0f, 0.0f);	// HP�� ������ ũ��
	D3DXVECTOR2 manaBarGaugeSize = D3DXVECTOR2(0.0f, 0.0f);	// Mana������ ũ��
};

class GaugeBarComponent : public IComponent
{
public:
	using IComponent::IComponent;
	~GaugeBarComponent() = default;

	// IComponent��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Destroy() override;

	void SetBasicState(float HP, float mana) { this->maxHP = HP, this->maxMana = mana; hp = maxHP; this->mana = maxMana; }
	void GetBasicState() { std::cout << this->GetActor()->GetName() << "�� ü�� : " << this->hp << ", ���� : " << this->mana; }

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
	class Timer* timer = nullptr;	// ������ �ð� ������ ���� ����

	// ���� �� ��ġ
	BarTransform stateBarTransform;
	GaugeScalePos gaugeScalePos;

	// ���� ���� ���� (ü�� ��, ���� ��, ü�� ������, ���� ������)
	std::map<std::string, std::shared_ptr<Actor>> children;

	// �ִ� ����
	float maxHP = 0;
	float maxMana = 0;

	// ���� ����
	float hp = 0;
	float mana = 0;
	bool isDead = false;
};

