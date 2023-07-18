#pragma once
#include "../IComponent.h"
#include "Core/SubSystem/Timer.h"

class AIScriptComponent final : public IComponent
{
public:
	using IComponent::IComponent;
	~AIScriptComponent() = default;

	// IComponent을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Destroy() override;

private:
	enum class Direction
	{
		Up,			// ↑
		UpRight,	// ↗
		Right,		// →
		RightDown,	// ↘
		Down,		// ↓
		DownLeft,	// ↙
		Left,		// ←
		LeftUp		// ↖
	};

private:
	Direction direction;						// 이동 방향
	StopWatch stopwatch;						// 이동 방향 변경 시간
};