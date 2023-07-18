#pragma once
#include "ISubSystem.h"

class GetCursorSystem : public ISubSystem
{
public:
	using ISubSystem::ISubSystem;
	~GetCursorSystem() = default;

	// ISubSystem을(를) 통해 상속됨
	virtual bool Initialize() override;
	virtual void Update() override;

private:
	static void GetWindowPos(int& absolMousePos, int& y);

public:
	static POINT mouse;
};