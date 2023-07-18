#include "stdafx.h"
#include "BackGroundManager.h"
#include "Scene/Actor.h"
#include "Resource/StaticTexture.h"

#include "../CameraComponent.h"
#include "../MoveComponent/TransformComponent.h"

void BackGroundComponent::Initialize()
{
	transform->SetScale(D3DXVECTOR3(Settings::Get().GetWidth(), Settings::Get().GetHeight(), 1));	// 객체 크기를 화면 전체로 설정
}

void BackGroundComponent::Update()
{
	// 카메라가 필요 없는 배경인 경우
	if (targetCamera == nullptr)
		return;

	// 카메라 위치로 배경 위치 동기화 (배경이 맨 아래로 이동하기 위해서 Z좌표를 -1로 지정
	transform->SetPosition(D3DXVECTOR3(targetCamera->GetTransform()->GetPosition().x,
									   targetCamera->GetTransform()->GetPosition().y, -1));
}

void BackGroundComponent::Destroy()
{
}

bool BackGroundComponent::SetMainCamera(std::shared_ptr<Actor> mainCamera)
{
	// 카메라를 불러옴
	auto cameraComponent = mainCamera.get()->GetComponent<CameraComponent>();

	// 해당 객체가 카메라가 있는지 확인
	if (cameraComponent == nullptr)
	{
		assert(false && "ERROR (Back1) : 카메라 컴퍼넌트를 찾을 수 없음");
		return false;
	}

	// 이 객체에 카메라의 컴퍼넌트를 연결함
	targetCamera = std::shared_ptr<CameraComponent>(cameraComponent);

	return true;
}

const std::shared_ptr<StaticTexture> BackGroundComponent::GetTexture() const
{
	return texture.expired() ? nullptr : texture.lock();
}

void BackGroundComponent::SetTexture(const std::string& path)
{
	texture = context->GetSubSystem<ResourceManager>()->Load<StaticTexture>(path);
}
