#pragma once

class D3D11_SamplerState final : public Object
{
public:
	D3D11_SamplerState(Graphics* graphics);
	~D3D11_SamplerState();

	ID3D11SamplerState* GetResource() const { return state; }

	void Create(const D3D11_FILTER& filter, const D3D11_TEXTURE_ADDRESS_MODE& address_mode);
	void Clear();

private:
	ID3D11Device* device = nullptr;
	ID3D11SamplerState* state = nullptr;
};