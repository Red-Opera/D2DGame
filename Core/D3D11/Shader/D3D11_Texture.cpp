#include "stdafx.h"
#include "D3D11_Texture.h"

D3D11_Texture::D3D11_Texture(Graphics* graphics)
{
	device = graphics->GetDevice();
}

D3D11_Texture::~D3D11_Texture()
{
	Clear();
}

void D3D11_Texture::Create(const std::string& path)
{
	HRESULT hr = D3DX11CreateShaderResourceViewFromFileA(device, path.c_str(), nullptr, nullptr, &shader_resource, nullptr);
	assert(SUCCEEDED(hr));
}

void D3D11_Texture::Clear()
{
	SAFE_RELEASE(shader_resource);
}