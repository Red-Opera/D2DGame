// WidePlainScene.h

#pragma once

#include "stdafx.h"
#include "Scene.h"

class WidePlainScene final : public Scene
{
public:
	WidePlainScene(class Context* const context);
	~WidePlainScene();

	void Update();
};