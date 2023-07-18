// MeshRendererComponent.h
// �������� ��ü�� Component�� �����ϴ� ���� (��� : Mesh(���)�� Material(����)�� �����ϰ� ��ȯ��)

enum class MeshType : uint
{
	Quad,			// �簢��
	ScreenQuad		// ��ũ�� ũ���� �簢��
};

#pragma once
#include "IComponent.h"

class MeshRendererComponent final : public IComponent
{
public:
	using IComponent::IComponent;
	~MeshRendererComponent() = default;

	// IComponent��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void Update() override;  
	virtual void Destroy() override;
	
	// ============================================
	// [Mesh]
	// ============================================
	std::shared_ptr<D3D11_VertexBuffer> GetVertexBuffer() const { return vertex_buffer; }
	std::shared_ptr<D3D11_IndexBuffer> GetIndexBuffer() const { return index_buffer; }
	void SetStandardMesh();		// vertex_buffer�� index_buffer�� �����ϴ� �Լ�

	// ============================================
	// [Material]
	// ============================================
	std::shared_ptr<D3D11_Shader> GetVertexShader() const { return vertex_shader; }
	std::shared_ptr<D3D11_Shader> GetPixelShader() const { return pixel_shader; }
	std::shared_ptr<D3D11_InputLayout> GetInputLayout() const { return input_layout; }
	void SetStandardMaterial();	// vertex_shader�� pixel_shader�� input_layout�� �����ϴ� �Լ�

private:
	MeshType mesh_type = MeshType::Quad;				// �Ϲ����� �簢�� ���·� ���

	std::shared_ptr<D3D11_VertexBuffer> vertex_buffer;	// ���� ������
	std::shared_ptr<D3D11_IndexBuffer> index_buffer;	// ���� ������ ���� ����
	std::shared_ptr<D3D11_InputLayout> input_layout;	// ���� ������ �Ӽ�
	std::shared_ptr<D3D11_Shader> vertex_shader;		// Shader�ڵ忡�� VS�� �ش��ϴ� �Լ� �ɼ��� �޴� ����
	std::shared_ptr<D3D11_Shader> pixel_shader;			// Shader�ڵ忡�� PS�� �ش��ϴ� �Լ� �ɼ��� �޴� ����
};