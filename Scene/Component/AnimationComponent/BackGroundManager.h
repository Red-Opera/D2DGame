#pragma once
#include "../IComponent.h"

class BackGroundComponent : public IComponent
{
public:
	using IComponent::IComponent;
	~BackGroundComponent() = default;

	// IComponent을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Destroy() override;


	// 배경이 따라갈 배경 설정
	bool SetMainCamera(std::shared_ptr<class Actor> mainCamera);

	const std::shared_ptr<class StaticTexture> GetTexture() const;		// StaticTexture(텍스쳐 정보)를 가져옴
	void SetTexture(const std::string& path);							// 경로로 텍스쳐를 가져옴

private:
	std::shared_ptr<class CameraComponent> targetCamera;		// 배경을 설치할 타겟 카메라
	std::weak_ptr<class StaticTexture> texture;					// 텍스텨에 관한 변수
};

