#include "stdafx.h"
#include "CameraComponent.h"
#include "MoveComponent/TransformComponent.h"

void CameraComponent::Initialize()
{
	// ī�޶� ��� ��ü ������ �����ϱ� ���� ���̸� 5�� ���� (ī�޶� �÷��̾ ������ �浹�ϴ� ������ �ذ�)
	transform->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 5.0f));

	D3DXMatrixIdentity(&view);
	D3DXMatrixIdentity(&projection);

	UpdateViewMatrix();
	UpdateProjectionMatrix();
}

void CameraComponent::Update()
{
	UpdateViewMatrix();
	UpdateProjectionMatrix();
}

void CameraComponent::Destroy()
{
}

void CameraComponent::UpdateViewMatrix()
{
	D3DXVECTOR3 position = transform->GetPosition();
	D3DXVECTOR3 at = position + transform->GetForward();
	D3DXVECTOR3 up = transform->GetUp();

	D3DXMatrixLookAtLH(&view, &position, &at, &up);
}

void CameraComponent::UpdateProjectionMatrix()
{
	D3DXMatrixOrthoLH(&projection, Settings::Get().GetWidth(), Settings::Get().GetHeight(), 0.0, 1.0f);
}