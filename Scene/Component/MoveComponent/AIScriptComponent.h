#pragma once
#include "../IComponent.h"
#include "Core/SubSystem/Timer.h"

class AIScriptComponent final : public IComponent
{
public:
	using IComponent::IComponent;
	~AIScriptComponent() = default;

	// IComponent��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Destroy() override;

private:
	enum class Direction
	{
		Up,			// ��
		UpRight,	// ��
		Right,		// ��
		RightDown,	// ��
		Down,		// ��
		DownLeft,	// ��
		Left,		// ��
		LeftUp		// ��
	};

private:
	Direction direction;						// �̵� ����
	StopWatch stopwatch;						// �̵� ���� ���� �ð�
};