// Time.h
// 스톱워치 관련 파일 (기능 : 스톱워치 시작, 초 또는 밀리초 구함)

#pragma once

#include "ISubSystem.h"

class Timer final : public ISubSystem
{
public:
	Timer(class Context* const context);
	~Timer() = default;

	// ISubSystem을(를) 통해 상속됨
	virtual bool Initialize() override;
	virtual void Update() override;

	const float GetDeltaTimeMS() { return static_cast<float>(delta_time_ms); }
	const float GetDeltaTimeSEC() { return static_cast<float>(delta_time_ms / 1000.0); }

private:
	double delta_time_ms = 0.0;		// Initialize부터 시간측정 시작
	std::chrono::time_point<std::chrono::high_resolution_clock> previous_time;
};

class StopWatch final
{
public:
	StopWatch();
	~StopWatch() = default;

	void Start();

	const float GetElapsedTimeSec();
	const float GetElapsedTimeMs();

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> start_time;	// 스톱워치 측정 시작 변수
};