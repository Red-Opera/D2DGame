// TraceComponent.h
// �ش� ��ü�� ã�ư��� ����

#pragma once
#include "../../Actor.h"
#include "../IComponent.h"

class TraceComponent : public IComponent
{
public:
	using IComponent::IComponent;

	TraceComponent() = default;
	~TraceComponent() = default;

	void SetTarget(std::weak_ptr<class Actor> target) { this->target = target; }	// �浹 ��ǥ ����
	std::weak_ptr<class Actor> GetTarget() { return target; }

	void SetSpeed(float speed) { this->speed = speed; }
	const float GetSpeed() const { return speed; }

	const Direction GetDirection() const { return this->direction; }
	const D3DXVECTOR2 GetToPos() const { return toPos; }

	// IComponent��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Destroy() override;

public:
	static bool FindRightMoveSprite(std::string& spriteName) { return (spriteName.substr(spriteName.size() - 9, 9) == "RightMove"); }
	static bool FindLeftMoveSprite(std::string& spriteName) { return (spriteName.substr(spriteName.size() - 8, 8) == "LeftMove"); }

private:
	std::weak_ptr<class Actor> target;		// Ÿ�� ��ü ����

	D3DXVECTOR3 targetPos;					// Ÿ�� ��ü�� ��ġ�� �޴� ����
	D3DXVECTOR3 targetSize;					// Ÿ�� ũ��

	Direction direction = Direction::Left;	// �̵� ����
	D3DXVECTOR2 toPos;						// �̵��� �߰����� ��ġ
	float speed = 0.0f;						// �̵� �ӵ�
};