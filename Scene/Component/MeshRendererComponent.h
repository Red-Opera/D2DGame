// MeshRendererComponent.h
// 랜더링할 객체를 Component를 세팅하는 파일 (기능 : Mesh(모양)와 Material(물질)을 설정하고 반환함)

enum class MeshType : uint
{
	Quad,			// 사각형
	ScreenQuad		// 스크린 크기의 사각형
};

#pragma once
#include "IComponent.h"

class MeshRendererComponent final : public IComponent
{
public:
	using IComponent::IComponent;
	~MeshRendererComponent() = default;

	// IComponent을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void Update() override;  
	virtual void Destroy() override;
	
	// ============================================
	// [Mesh]
	// ============================================
	std::shared_ptr<D3D11_VertexBuffer> GetVertexBuffer() const { return vertex_buffer; }
	std::shared_ptr<D3D11_IndexBuffer> GetIndexBuffer() const { return index_buffer; }
	void SetStandardMesh();		// vertex_buffer와 index_buffer을 세팅하는 함수

	// ============================================
	// [Material]
	// ============================================
	std::shared_ptr<D3D11_Shader> GetVertexShader() const { return vertex_shader; }
	std::shared_ptr<D3D11_Shader> GetPixelShader() const { return pixel_shader; }
	std::shared_ptr<D3D11_InputLayout> GetInputLayout() const { return input_layout; }
	void SetStandardMaterial();	// vertex_shader와 pixel_shader와 input_layout을 세팅하는 함수

private:
	MeshType mesh_type = MeshType::Quad;				// 일반적인 사각형 형태로 출력

	std::shared_ptr<D3D11_VertexBuffer> vertex_buffer;	// 정점 데이터
	std::shared_ptr<D3D11_IndexBuffer> index_buffer;	// 정점 데이터 연결 순서
	std::shared_ptr<D3D11_InputLayout> input_layout;	// 정점 데이터 속성
	std::shared_ptr<D3D11_Shader> vertex_shader;		// Shader코드에서 VS에 해당하는 함수 옵션을 받는 변수
	std::shared_ptr<D3D11_Shader> pixel_shader;			// Shader코드에서 PS에 해당하는 함수 옵션을 받는 변수
};