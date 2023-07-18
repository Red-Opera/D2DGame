// Engine.h
// 모든 매니저 등록하고 실행시켜주는 파일

#pragma once

class Engine
{
public:
	Engine();
	~Engine();

	void Update();
	void Render();

private:
	class Context* context = nullptr;	// 매니저를 등록하고 실행하기 위한 변수
};