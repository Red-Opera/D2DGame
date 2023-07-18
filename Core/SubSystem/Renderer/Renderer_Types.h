// Renderer_Types.h

#pragma once

#include "stdafx.h"

// 랜더하는 객체를 정의하는 Enum
enum class RenderableType : uint
{
	Opaque, // 불투명
	Camera
};

// 도형의 출력면 및 와이어 프레임 또는 일반 출력 Enum
enum class RasterizerStateType : uint
{
	Min,
	Cull_Front_Fill_Solid = Min,
	Cull_Back_Fill_Solid,
	Cull_None_Fill_Solid,
	Cull_Front_Fill_Wirefame,
	Cull_Back_Fill_Wirefame,
	Cull_None_Fill_Wirefame,
	Max = Cull_None_Fill_Wirefame
};

// 알파 값으로 배경과 섞어 투명하게 만들어 주는 Enum
enum class BlendStateType : uint
{
	Min,
	Disabled = Min,
	Alpha,
	Max = Alpha
};