// D3D11_VertexBuffer.h
// IA단계의 Vertex Buffer를 설정하는 파일 (정점들의 데이터를 넣어줌)

#pragma once

class D3D11_VertexBuffer final : public Object
{
public:
	D3D11_VertexBuffer(Graphics* graphics);
	~D3D11_VertexBuffer();

	ID3D11Buffer* GetResource() const { return buffer; }
	const uint& GetStride() const { return stride; }
	const uint& GetOffset() const { return offset; }
	const uint& GetCount() const { return count; }

	template<typename T>
	void Create(const std::vector<T>& vertices, const D3D11_USAGE& usage = D3D11_USAGE_IMMUTABLE);

	void Clear();

private:
	ID3D11Device* device = nullptr;		// 정점 데이터와 텍스처와 같은 자원을 생성하기 위한 변수 
	ID3D11Buffer* buffer = nullptr;		// GPU에 데이터를 보내기 위한 버퍼

	uint stride = 0;					// 정점 1개당 데이터 크기
	uint offset = 0;					// 정점 데이터 시작 위치
	uint count = 0;						// 정점 개수
};

template<typename T>
inline void D3D11_VertexBuffer::Create(const std::vector<T>& vertices, const D3D11_USAGE& usage)
{
	stride = sizeof(T);
	count = static_cast<uint>(vertices.size());

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

	desc.ByteWidth = stride * count;
	desc.Usage = usage;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	switch (usage)
	{
	case D3D11_USAGE_DEFAULT:
	case D3D11_USAGE_IMMUTABLE:
	{
		desc.CPUAccessFlags = 0;
		break;
	}

	case D3D11_USAGE_DYNAMIC:
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		break;

	case D3D11_USAGE_STAGING:
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
		break;

	default:
		break;
	}

	D3D11_SUBRESOURCE_DATA sub_data;
	ZeroMemory(&sub_data, sizeof(D3D11_SUBRESOURCE_DATA));
	sub_data.pSysMem = vertices.data();

	HRESULT hr = device->CreateBuffer(&desc, &sub_data, &buffer);
	assert(SUCCEEDED(hr));
}