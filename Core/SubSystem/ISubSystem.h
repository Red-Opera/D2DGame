// ISubSystem.h
// SubSystem�� �Ŵ����� �⺻���� Ʋ�� �����ϰ� context�� ��ȯ�ϴ� ����

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
	class Context* context = nullptr;	// context�� ��ȯ�ϱ� ���� ����
};