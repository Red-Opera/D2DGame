#pragma once
#include "ISubSystem.h"

class ScoreUI final : public ISubSystem
{
public:
	using ISubSystem::ISubSystem;
	~ScoreUI() = default;

	// ISubSystem을(를) 통해 상속됨
	virtual bool Initialize() override;
	virtual void Update() override;

	void AddScore(int score) { this->score += score; }
	
	unsigned long long GetScore() { return score; }

private:
	unsigned long long score;
	bool firstRun = false;

	std::unordered_map<std::string, std::shared_ptr<class Actor>> UIActor;
};