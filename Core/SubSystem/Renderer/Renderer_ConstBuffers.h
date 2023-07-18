// Renderer_ConstBuffers.h
// 랜더링에 필요한 ConstBuffer를 하나로 묶어주기 위한 파일

#pragma once
#include "stdafx.h"

struct CAMERA_DATA
{
	D3DXMATRIX view, projection;
};

struct TRANSFORM_DATA
{
	D3DXMATRIX world;
};

struct ANIMATION_DATA
{
	D3DXVECTOR2 sprite_offset;
	D3DXVECTOR2 sprite_size;	// 스프라이트 사진 사이즈
	D3DXVECTOR2 texture_size;	// 텍스쳐 사이즈
	float isTexture;			// 텍스쳐인지 여부
	float padding;
};
