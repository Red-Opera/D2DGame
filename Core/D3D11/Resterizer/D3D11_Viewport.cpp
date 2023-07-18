// D3D11_Viewport.cpp

#include "stdafx.h"
#include "D3D11_Viewport.h"

D3D11_Viewport::D3D11_Viewport(const float& x, const float& y, const float& z, const float& width, const float& height, const float& min_depth, const float& max_depth)
	: x(x), y(y), width(width), height(height), min_depth(min_depth), max_depth(max_depth)
{
}

D3D11_Viewport::D3D11_Viewport(const D3D11_Viewport& rhs)
	: x(rhs.x), y(rhs.y), width(rhs.width), height(rhs.height), min_depth(rhs.min_depth), max_depth(rhs.max_depth)
{
}