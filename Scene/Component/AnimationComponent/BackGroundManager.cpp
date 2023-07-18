#include "stdafx.h"
#include "BackGroundManager.h"
#include "Scene/Actor.h"
#include "Resource/StaticTexture.h"

#include "../CameraComponent.h"
#include "../MoveComponent/TransformComponent.h"

void BackGroundComponent::Initialize()
{
	transform->SetScale(D3DXVECTOR3(Settings::Get().GetWidth(), Settings::Get().GetHeight(), 1));	// ��ü ũ�⸦ ȭ�� ��ü�� ����
}

void BackGroundComponent::Update()
{
	// ī�޶� �ʿ� ���� ����� ���
	if (targetCamera == nullptr)
		return;

	// ī�޶� ��ġ�� ��� ��ġ ����ȭ (����� �� �Ʒ��� �̵��ϱ� ���ؼ� Z��ǥ�� -1�� ����
	transform->SetPosition(D3DXVECTOR3(targetCamera->GetTransform()->GetPosition().x,
									   targetCamera->GetTransform()->GetPosition().y, -1));
}

void BackGroundComponent::Destroy()
{
}

bool BackGroundComponent::SetMainCamera(std::shared_ptr<Actor> mainCamera)
{
	// ī�޶� �ҷ���
	auto cameraComponent = mainCamera.get()->GetComponent<CameraComponent>();

	// �ش� ��ü�� ī�޶� �ִ��� Ȯ��
	if (cameraComponent == nullptr)
	{
		assert(false && "ERROR (Back1) : ī�޶� ���۳�Ʈ�� ã�� �� ����");
		return false;
	}

	// �� ��ü�� ī�޶��� ���۳�Ʈ�� ������
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
