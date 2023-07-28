// MainScene.h

#pragma once

#include "stdafx.h"
#include "Scene.h"

class MainScene final : public Scene
{
public:
	MainScene(class Context* const context);
	~MainScene();
	
	void Update();

private:
	Timer* timer = 0;						// 타임머
	float nowDelay = 0;						// 다음 몬스터 생성까지 남은 시간
	unsigned int onMonster = 0;				// 활성시킨 몬스터 수
};