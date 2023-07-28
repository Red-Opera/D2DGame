// 모든 Renderer를 하나로 묶어주어 관리해주는 파일

#pragma once
#include "../ISubSystem.h"

#include "Renderer_ConstBuffers.h"
#include "Renderer_Types.h"

class Renderer final : public ISubSystem
{
public:
	using ISubSystem::ISubSystem;
	~Renderer();

	// ISubSystem을(를) 통해 상속됨
	virtual bool Initialize() override;
	virtual void Update() override;

	void UpdateRenderables(class Scene* const scene);	// renderables의 모든 객체를 업데이트 해주는 함수
	void SetCameraTarget(class CameraComponent* camera) { this->camera = camera; }

private:
	void PassMain();	// 해당 파일아래에 함수를 정의하고 구현을 누르면 Rederer_Passes로 이동함

private:
	class Graphics* graphics = nullptr;					// 많이 사용하여 선언
	class CameraComponent* camera = nullptr;			// 메인 카메라
	std::shared_ptr<class D3D11_Pipeline> pipeline;		// 랜더링을 위한 파이프라인

	// 카메라의 상수 버퍼
	CAMERA_DATA cpu_camera_buffer;
	std::shared_ptr<class D3D11_ConstantBuffer> gpu_camera_buffer;

	// 객체의 상수 버퍼
	TRANSFORM_DATA cpu_object_buffer;
	std::shared_ptr<class D3D11_ConstantBuffer> gpu_object_buffer;

	// 에니메이션 관련 상수 버퍼
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

	std::unordered_map<RenderableType, std::vector<class Actor*>> renderables;	// 해당 타입에 따른 객체 저장

};