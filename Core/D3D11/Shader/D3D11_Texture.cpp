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
	// ���� �ؽ��ĸ� ���������� �ִٸ� ����Ʈ���� ������
	if (TempTexture::HasTexture(path))
		shader_resource = TempTexture::GetTexture(path);

	else
	{
		// �ش� �ؽ��ĸ� ������ ���� ���ٸ� �̹����� ������
		HRESULT hr = D3DX11CreateShaderResourceViewFromFileA(device, path.c_str(), nullptr, nullptr, &shader_resource, nullptr);
		assert(SUCCEEDED(hr));

		// shader_resource ���
		TempTexture::SetTexture(path, shader_resource);
	}
}

void D3D11_Texture::Clear()
{
	shader_resource = nullptr;
}