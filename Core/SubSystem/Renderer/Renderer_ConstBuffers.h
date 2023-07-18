// Renderer_ConstBuffers.h
// �������� �ʿ��� ConstBuffer�� �ϳ��� �����ֱ� ���� ����

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
	D3DXVECTOR2 sprite_size;	// ��������Ʈ ���� ������
	D3DXVECTOR2 texture_size;	// �ؽ��� ������
	float isTexture;			// �ؽ������� ����
	float padding;
};
