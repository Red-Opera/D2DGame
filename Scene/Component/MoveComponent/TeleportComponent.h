#pragma once
#include "../IComponent.h"

class TeleportComponent : public IComponent
{
public:
	using IComponent::IComponent;
	~TeleportComponent() = default;

	// IComponent을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Destroy() override;

	void SetNextMap(std::string mapName) { this->mapName = mapName; }
	std::string GetNextMap() { return mapName; }

private:
	std::string mapName;
	std::weak_ptr<class Actor> player;
	std::weak_ptr<class CollisionComponent> collsion;
	
	class Actor* parent;
	class Scene* scene;
};

