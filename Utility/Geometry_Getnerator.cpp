#include "stdafx.h"
#include "Geometry_Getnerator.h"

void Geometry_Getnerator::CreateQuad(D3D11_Geometry<struct D3D11_VertexTexture>& geometry)
{
	geometry.AddVertex(D3D11_VertexTexture(D3DXVECTOR3(-0.5f, -0.5f, 0.0f), D3DXVECTOR2(0.0f, 1.0f)));
	geometry.AddVertex(D3D11_VertexTexture(D3DXVECTOR3(-0.5f, +0.5f, 0.0f), D3DXVECTOR2(0.0f, 0.0f)));
	geometry.AddVertex(D3D11_VertexTexture(D3DXVECTOR3(+0.5f, -0.5f, 0.0f), D3DXVECTOR2(1.0f, 1.0f)));
	geometry.AddVertex(D3D11_VertexTexture(D3DXVECTOR3(+0.5f, +0.5f, 0.0f), D3DXVECTOR2(1.0f, 0.0f)));

	geometry.AddIndex(0); geometry.AddIndex(1); geometry.AddIndex(2);
	geometry.AddIndex(2); geometry.AddIndex(1); geometry.AddIndex(3);
}

void Geometry_Getnerator::CreateQuad(D3D11_Geometry<struct D3D11_VertexColor>& geometry, const D3DXCOLOR& color)
{
	geometry.AddVertex(D3D11_VertexColor(D3DXVECTOR3(-0.5f, -0.5f, 0.0f), color));
	geometry.AddVertex(D3D11_VertexColor(D3DXVECTOR3(-0.5f, +0.5f, 0.0f), color));
	geometry.AddVertex(D3D11_VertexColor(D3DXVECTOR3(+0.5f, -0.5f, 0.0f), color));
	geometry.AddVertex(D3D11_VertexColor(D3DXVECTOR3(+0.5f, +0.5f, 0.0f), color));

	geometry.AddIndex(0); geometry.AddIndex(1); geometry.AddIndex(2);
	geometry.AddIndex(2); geometry.AddIndex(1); geometry.AddIndex(3);
}
