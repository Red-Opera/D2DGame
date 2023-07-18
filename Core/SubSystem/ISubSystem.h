// ISubSystem.h
// SubSystem인 매니저의 기본적인 틀을 정의하고 context를 반환하는 파일

#pragma once

class ISubSystem
{
public:
	ISubSystem(class Context* context) : context(context) {}
	virtual ~ISubSystem() = default;

	virtual bool Initialize() = 0;
	virtual void Update() = 0;

	const class Context* const GetContext() const { return context; }

protected:
	class Context* context = nullptr;	// context를 반환하기 위한 변수
};