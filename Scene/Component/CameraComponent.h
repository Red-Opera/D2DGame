#pragma once
#include "IComponent.h"

class CameraComponent final : public IComponent
{
public:
	using IComponent::IComponent;
	CameraComponent();
	~CameraComponent() = default;

	// IComponent을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Destroy() override;

	const D3DXMATRIX& GetViewMatrix() const { return view; }
	const D3DXMATRIX& GetProjectionMatrix() const { return projection; }

private:
	void UpdateViewMatrix();
	void UpdateProjectionMatrix();

private:
	D3DXMATRIX view;
	D3DXMATRIX projection;
};