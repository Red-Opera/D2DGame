#pragma once
#include "ISubSystem.h"

class SceneManager final : public ISubSystem
{
public:
	using ISubSystem::ISubSystem;
	~SceneManager() = default;

	// ISubSystem을(를) 통해 상속됨
	virtual bool Initialize() override;
	virtual void Update() override;

	const std::map<std::string, std::shared_ptr<class Scene>>& GetAllScenes() const { return scenes; }
	
	class Scene* const GetCurrentScene();
	std::string GetCurrentSceneName() { return nowSceneName; }

	void SetCurrentScene(const std::string& scene_name);
	void SetCurrentSceneName(std::string name) { nowSceneName = name; }

	class Scene* const RegisterScene(const std::string& scene_name);

private:
	std::map<std::string, std::shared_ptr<class Scene>> scenes;
	std::weak_ptr<Scene> current_scene;
	std::string nowSceneName;
};