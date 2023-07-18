#pragma once

class D3D11_InputLayout final : public Object
{
public:
	D3D11_InputLayout(Graphics* graphics);
	~D3D11_InputLayout();

	ID3D11InputLayout* GetResource() const { return input_layout; }

	void Create(D3D11_INPUT_ELEMENT_DESC* desc, const uint& count, ID3DBlob* blob);
	void Clear();

private:
	ID3D11Device* device = nullptr;
	ID3D11InputLayout* input_layout = nullptr;
};