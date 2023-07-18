#include "stdafx.h"
#include "D3D11_SamplerState.h"

D3D11_SamplerState::D3D11_SamplerState(Graphics* graphics)
{
	device = graphics->GetDevice();
}

D3D11_SamplerState::~D3D11_SamplerState()
{
	Clear();
}

void D3D11_SamplerState::Create(const D3D11_FILTER& filter, const D3D11_TEXTURE_ADDRESS_MODE& address_mode)
{
	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_SAMPLER_DESC));

	desc.Filter = filter;
	desc.AddressU = address_mode;
	desc.AddressV = address_mode;
	desc.AddressW = address_mode;
	desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	desc.MaxAnisotropy = 16;
	desc.BorderColor[0] = 0.0f;
	desc.BorderColor[1] = 0.0f;
	desc.BorderColor[2] = 0.0f;
	desc.BorderColor[3] = 0.0f;
	desc.MinLOD = std::numeric_limits<float>::min();
	desc.MaxLOD = std::numeric_limits<float>::max();
	desc.MipLODBias = 0.0f;

	HRESULT hr = device->CreateSamplerState(&desc, &state);
	assert(SUCCEEDED(hr));
}

void D3D11_SamplerState::Clear()
{
	SAFE_RELEASE(state);
}
