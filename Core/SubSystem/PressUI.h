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

	void Reflash() { changeScene = true; }
private:
	bool firstRun = false;
	bool changeScene = false;

	std::unordered_map<std::string, std::shared_ptr<class Actor>> UIActor;
};