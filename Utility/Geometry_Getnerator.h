#pragma once

template<typename T> class D3D11_Geometry;

class Geometry_Getnerator final
{
public:
	static void CreateQuad(D3D11_Geometry<struct D3D11_VertexTexture>& geometry);
	static void CreateQuad(D3D11_Geometry<struct D3D11_VertexColor>& geometry, const D3DXCOLOR& color);
};