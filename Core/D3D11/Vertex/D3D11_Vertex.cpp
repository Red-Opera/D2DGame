/*
	절대 경로 : 최상위 시작지점부터 목표지점까지를 가리키는 절대적인 경로
	상대 경로 : 기준 위치를 잡아 그 기준을 상대로 목표지점을 가리키는 경로
		-> 추가 포함 디렉터리에 추가하면 상대경로의 기준위치를 추가해줄 수 있다.
*/

#include "stdafx.h"
#include "D3D11_Vertex.h"

D3D11_INPUT_ELEMENT_DESC D3D11_VertexColor::descs[]
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0,							   0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

D3D11_INPUT_ELEMENT_DESC D3D11_VertexTexture::descs[]
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0,							   0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};