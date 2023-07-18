// Renderer_Types.h

#pragma once

#include "stdafx.h"

// �����ϴ� ��ü�� �����ϴ� Enum
enum class RenderableType : uint
{
	Opaque, // ������
	Camera
};

// ������ ��¸� �� ���̾� ������ �Ǵ� �Ϲ� ��� Enum
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

// ���� ������ ���� ���� �����ϰ� ����� �ִ� Enum
enum class BlendStateType : uint
{
	Min,
	Disabled = Min,
	Alpha,
	Max = Alpha
};