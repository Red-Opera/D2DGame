#pragma once

#include "../IComponent.h"

class MoveScriptComponent final : public IComponent
{
public:
	using IComponent::IComponent;
	~MoveScriptComponent() = default;

	// IComponent을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Destroy() override;

	void SetMoveSpeed(float speed) { this->speed = speed; }
	float GetMoveSpeed() { return speed; }

private:
	float speed = 0.0f;
};