#pragma once
#include "../IComponent.h"

class BackGroundComponent : public IComponent
{
public:
	using IComponent::IComponent;
	~BackGroundComponent() = default;

	// IComponent��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Destroy() override;


	// ����� ���� ��� ����
	bool SetMainCamera(std::shared_ptr<class Actor> mainCamera);

	const std::shared_ptr<class StaticTexture> GetTexture() const;		// StaticTexture(�ؽ��� ����)�� ������
	void SetTexture(const std::string& path);							// ��η� �ؽ��ĸ� ������

private:
	std::shared_ptr<class CameraComponent> targetCamera;		// ����� ��ġ�� Ÿ�� ī�޶�
	std::weak_ptr<class StaticTexture> texture;					// �ؽ��߿� ���� ����
};

