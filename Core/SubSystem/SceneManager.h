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
	void SetCurrentScene(const std::string& scene_name);
	class Scene* const RegisterScene(const std::string& scene_name);

private:
	std::map<std::string, std::shared_ptr<class Scene>> scenes;
	std::weak_ptr<Scene> current_scene;

	/*
		Vector 장점
			- 가변 배열
			- 임의 접근 가능(빠른 데이터 접근)
			- 데이터를 순차적으로 처리하는게 빠름

		Vector 단점
			- 중간 원소에 대한 삽입/삭제 불리
			- 크기가 예측이 안될 경우 재할당이 많이 발생함
			- 정렬되지 않는 상태에서의 탐색이 불리함
	*/
};