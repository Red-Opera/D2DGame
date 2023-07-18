#pragma once

#include "D3D11_Geometry.h"

template<typename T>
inline D3D11_Geometry<T>::~D3D11_Geometry()
{
	Clear();
}

template<typename T>
inline const std::vector<T> D3D11_Geometry<T>::GetVertices(const uint& offset, const uint& count)
{
	std::vector<T> sub_vertices;				// 사용자가 입력한 범위에 값을 복사하여 리턴

	auto first = vertices.begin() + offset;		// 복사를 시작할 원소의 위치 지정
	auto last = first + count;					// 복사 시작 위치에서부터 읽어야 하는 마지막 원소의 다음 위치

	sub_vertices.assign(first, last);			// assign() : first부터 last전까지의 원소를 복사하여 할당해주는 함수

	return sub_vertices;
}

template<typename T>
inline void D3D11_Geometry<T>::AddVertex(const T& vertex)
{
	vertices.emplace_back(vertex);
}

template<typename T>
inline void D3D11_Geometry<T>::AddVertices(const std::vector<T>& vertices)
{
	this->vertices.insert(this->vertices.end(), vertices.begin(), vertices.end());
}

template<typename T>
inline void D3D11_Geometry<T>::SetVertices(const std::vector<T>& vertices)
{
	this->vertices.clear();
	this->vertices.shrink_to_fit();

	this->vertices = vertices;
}

template<typename T>
inline const std::vector<uint> D3D11_Geometry<T>::GetIndices(const uint& offset, const uint& count)
{
	std::vector<uint> sub_indices;	

	auto first = indices.begin() + offset;
	auto last = first + count;	

	sub_indices.assign(first, last);

	return sub_indices;
}

template<typename T>
inline void D3D11_Geometry<T>::AddIndex(const uint& index)
{
	indices.emplace_back(index);
}

template<typename T>
inline void D3D11_Geometry<T>::AddIndices(const std::vector<uint>& indices)
{
	this->indices.insert(this->indices.end(), indices.begin(), indices.end());
}

template<typename T>
inline void D3D11_Geometry<T>::SetIndices(const std::vector<uint>& indices)
{
	this->indices.clear();
	this->indices.shrink_to_fit();

	this->indices = indices;
}

template<typename T>
inline void D3D11_Geometry<T>::Clear()
{
	// clear() : 백터의 모든 원소 삭제, 메모리 공간은 유지
	// shrink_to_fit() : 메모리 공간을 원소의 개수만큼 맞춰준다. -> 여분 메모리 제거

	vertices.clear();
	vertices.shrink_to_fit();

	indices.clear();
	indices.shrink_to_fit();
}