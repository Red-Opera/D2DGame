#include "stdafx.h"
#include "D3D11_InputLayout.h"

D3D11_InputLayout::D3D11_InputLayout(Graphics* graphics)
{
	device = graphics->GetDevice();
}

D3D11_InputLayout::~D3D11_InputLayout()
{
	Clear();
}

void D3D11_InputLayout::Create(D3D11_INPUT_ELEMENT_DESC* desc, const uint& count, ID3DBlob* blob)
{
	if (desc == nullptr || count <= 0 || blob == nullptr)
	{
		assert(false);
		return;
	}

	HRESULT hr = device->CreateInputLayout(desc, count, blob->GetBufferPointer(), blob->GetBufferSize(), &input_layout);
	assert(SUCCEEDED(hr));
}

void D3D11_InputLayout::Clear()
{
	SAFE_RELEASE(input_layout);
}
