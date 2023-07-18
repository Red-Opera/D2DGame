#pragma once

class D3D11_Texture final : public Object
{
public:
	D3D11_Texture(Graphics* graphics);
	~D3D11_Texture();

	ID3D11ShaderResourceView* GetResource() const { return shader_resource; }

	void Create(const std::string& path);
	void Clear();

private:
	ID3D11Device* device = nullptr;
	ID3D11ShaderResourceView* shader_resource = nullptr;
};