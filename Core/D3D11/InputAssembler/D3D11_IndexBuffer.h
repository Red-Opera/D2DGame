// D3D11_IndexBuffer.h
// IA단계의 Index Buffer을 세팅하는 파일 (정점 데이터의 연결 순서를 정함)

#pragma once

class D3D11_IndexBuffer final : public Object
{
public:
	D3D11_IndexBuffer(Graphics* graphics);
	~D3D11_IndexBuffer();

	ID3D11Buffer* GetResource() const { return buffer; }
	const uint& GetStride() const { return stride; }
	const uint& GetOffset() const { return offset; }
	const uint& GetCount() const { return count; }

	void Create(const std::vector<uint>& indices, const D3D11_USAGE& usage = D3D11_USAGE_IMMUTABLE);

	void Clear();

private:
	ID3D11Device* device = nullptr;
	ID3D11Buffer* buffer = nullptr;

	uint stride = 0;
	uint offset = 0;
	uint count = 0;
};