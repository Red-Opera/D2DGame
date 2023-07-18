// D3D11_Viewport.h
// RS단계의 Viewport를 클래스로 정의하는 파일 (화면에 그릴 위치나 영역을 설정)

#pragma once

class D3D11_Viewport final : public Object
{
public:
	D3D11_Viewport(const float& x = 0.0f, const float& y = 0.0f, const float& z = 0.0f, 
				   const float& width = 0.0f, const float& height = 0.0f, const float& min_depth = 0.0f, const float& max_depth = 1.0f);
	D3D11_Viewport(const D3D11_Viewport& rhs);
	~D3D11_Viewport() = default;

public:
	float x = 0.0f; float y = 0.0f; float z = 0.0f;												
	float width = 0.0f; float height = 0.0f; float min_depth = 0.0f; float max_depth = 0.0f;
};