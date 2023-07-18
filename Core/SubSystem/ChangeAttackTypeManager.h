#pragma once
#include "ISubSystem.h"

class ChangeAttackTypeManager : public ISubSystem
{
public:
	using ISubSystem::ISubSystem;
	~ChangeAttackTypeManager() = default;

	// ISubSystem을(를) 통해 상속됨
	virtual bool Initialize() override;
	virtual void Update() override;

	static void ChangeType(std::shared_ptr<class Actor> target1, std::queue<int> key1, std::queue<enum class PlayerBulletStyle> style1, std::queue<float> bulletSpeed1);
	
public:
	const class InputManager* input = nullptr;
	
	static std::shared_ptr<class Actor> target;

	static std::queue<int> key;
	static std::queue<enum class PlayerBulletStyle> style;
	static std::queue<float> bulletSpeed;
};