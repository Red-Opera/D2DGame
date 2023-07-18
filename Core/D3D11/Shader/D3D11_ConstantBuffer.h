#pragma once

class D3D11_ConstantBuffer final : public Object
{
public:
	D3D11_ConstantBuffer(Graphics* graphics);
	~D3D11_ConstantBuffer();

	ID3D11Buffer* GetResource() const { return buffer; }

	template<typename T>
	void Create(const D3D11_USAGE& usage = D3D11_USAGE_DYNAMIC);
	void Clear();

	template<typename T>
	T* Map();
	bool Unmap();

private:
	ID3D11Device* device = nullptr;
	ID3D11DeviceContext* device_context = nullptr;
	ID3D11Buffer* buffer = nullptr;
};

template<typename T>
inline void D3D11_ConstantBuffer::Create(const D3D11_USAGE& usage)
{
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

	desc.ByteWidth = sizeof(T);
	desc.Usage = usage;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	switch (usage)
	{
	case D3D11_USAGE_DEFAULT:
	case D3D11_USAGE_IMMUTABLE: desc.CPUAccessFlags = 0; break;
	case D3D11_USAGE_DYNAMIC:   desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; break;
	case D3D11_USAGE_STAGING:   desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE; break;
	default:                    break;
	}

	HRESULT hr = device->CreateBuffer(&desc, nullptr, &buffer);
	assert(SUCCEEDED(hr));
}

template<typename T>
inline T* D3D11_ConstantBuffer::Map()
{
	D3D11_MAPPED_SUBRESOURCE mapped_resource;
	HRESULT hr = device_context->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);
	assert(SUCCEEDED(hr));

	return static_cast<T*>(mapped_resource.pData);
}
