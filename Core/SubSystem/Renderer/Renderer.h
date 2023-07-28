// ��� Renderer�� �ϳ��� �����־� �������ִ� ����

#pragma once
#include "../ISubSystem.h"

#include "Renderer_ConstBuffers.h"
#include "Renderer_Types.h"

class Renderer final : public ISubSystem
{
public:
	using ISubSystem::ISubSystem;
	~Renderer();

	// ISubSystem��(��) ���� ��ӵ�
	virtual bool Initialize() override;
	virtual void Update() override;

	void UpdateRenderables(class Scene* const scene);	// renderables�� ��� ��ü�� ������Ʈ ���ִ� �Լ�
	void SetCameraTarget(class CameraComponent* camera) { this->camera = camera; }

private:
	void PassMain();	// �ش� ���ϾƷ��� �Լ��� �����ϰ� ������ ������ Rederer_Passes�� �̵���

private:
	class Graphics* graphics = nullptr;					// ���� ����Ͽ� ����
	class CameraComponent* camera = nullptr;			// ���� ī�޶�
	std::shared_ptr<class D3D11_Pipeline> pipeline;		// �������� ���� ����������

	// ī�޶��� ��� ����
	CAMERA_DATA cpu_camera_buffer;
	std::shared_ptr<class D3D11_ConstantBuffer> gpu_camera_buffer;

	// ��ü�� ��� ����
	TRANSFORM_DATA cpu_object_buffer;
	std::shared_ptr<class D3D11_ConstantBuffer> gpu_object_buffer;

	// ���ϸ��̼� ���� ��� ����
	ANIMATION_DATA cpu_animation_buffer;
	std::shared_ptr<class D3D11_ConstantBuffer> gpu_animation_buffer;

private:
	void CreateConstantBuffers();
	void CreateRasterizerStates();
	void CreateBlendStates();

	void UpdateCameraBuffer();
	void UpdateObjectBuffer();
	void UpdateAnimationBuffer();

private:
	std::map<RasterizerStateType, std::shared_ptr<class D3D11_RasterizerState>> rasterizers;
	std::map<BlendStateType, std::shared_ptr<class D3D11_Blend_state>> blend_states;

	std::unordered_map<RenderableType, std::vector<class Actor*>> renderables;	// �ش� Ÿ�Կ� ���� ��ü ����

};