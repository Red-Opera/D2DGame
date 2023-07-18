#pragma once
// D3D11_Geometry.h

template<typename T>
class D3D11_Geometry final
{
public:
	D3D11_Geometry() = default;
	~D3D11_Geometry();

	// ===============================================================
	//	[Vertex]
	// ===============================================================
	const uint GetVertexCount() const { return static_cast<uint>(vertices.size()); }	// vertices의 크기를 반환하는 기능
	const uint GetVertexByteWidth() const { return GetVertexCount() * sizeof(T); }		// 바이트 크기를 반환하는 기능
	const T* GetVertexPointer() const { return vertices.data(); }						// vertices의 주소를 가져오는 기능
	const std::vector<T>& GetVertices() const { return vertices; }						// vertices 전체를 가져오는 기능
	const std::vector<T> GetVertices(const uint& offset, const uint& count);			// 특정 지점만 가져오는 기능
	
	// Add : 기존 데이터 추가
	// Set : 기존 데이터를 버리고 새로운 데이터 세팅
	void AddVertex(const T& vertex);					
	void AddVertices(const std::vector<T>& vertices);									
	void SetVertices(const std::vector<T>& vertices);							

	// ===============================================================
	//	[Index]
	// ===============================================================
	const uint GetIndexCount() const { return static_cast<uint>(indices.size()); }
	const uint GetIndexByteWidth() const { return GetIndexCount() * sizeof(uint); }	
	const uint* GetIndexPointer() const { return indices.data(); }
	const std::vector<uint>& GetIndices() const { return indices; }
	const std::vector<uint> GetIndices(const uint& offset, const uint& count);

	void AddIndex(const uint& index);
	void AddIndices(const std::vector<uint>& indices);
	void SetIndices(const std::vector<uint>& indices);

	// ===============================================================
	//	[MICS]
	// ===============================================================

	void Clear();


private:
	std::vector<T> vertices;
	std::vector<uint> indices;
};

#include "D3D11_Geometry.inl"