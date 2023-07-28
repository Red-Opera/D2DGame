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
	Timer* timer = 0;						// Ÿ�Ӹ�
	float nowDelay = 0;						// ���� ���� �������� ���� �ð�
	unsigned int onMonster = 0;				// Ȱ����Ų ���� ��
};