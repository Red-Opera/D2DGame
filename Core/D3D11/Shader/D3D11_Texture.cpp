#include "stdafx.h"
#include "D3D11_Texture.h"
#include "TempTexture.h"

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
	// 만약 텍스쳐를 가져온적이 있다면 리스트에서 가져옴
	if (TempTexture::HasTexture(path))
		shader_resource = TempTexture::GetTexture(path);

	else
	{
		// 해당 텍스쳐를 가져온 적이 없다면 이미지를 가져옴
		HRESULT hr = D3DX11CreateShaderResourceViewFromFileA(device, path.c_str(), nullptr, nullptr, &shader_resource, nullptr);
		assert(SUCCEEDED(hr));

		// shader_resource 등록
		TempTexture::SetTexture(path, shader_resource);
	}
}

void D3D11_Texture::Clear()
{
	shader_resource = nullptr;
}