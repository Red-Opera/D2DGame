#pragma once
#include "ISubSystem.h"

class PressUI : public ISubSystem
{
public:
	using ISubSystem::ISubSystem;
	~PressUI() = default;

	// ISubSystem��(��) ���� ��ӵ�
	virtual bool Initialize() override;
	virtual void Update() override;
private:
	bool firstRun = false;

	std::unordered_map<std::string, std::shared_ptr<class Actor>> UIActor;
};