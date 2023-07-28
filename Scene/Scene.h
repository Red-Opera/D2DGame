// Scene.h

#pragma once

class Scene
{
public:
	Scene(class Context* const context);
	~Scene();

	void Update();

	const std::shared_ptr<class Actor> CreateActor(const bool& is_active = true);			// Actor를 추가하고 바로 활성화하는 함수
	void AddActor(const std::shared_ptr<class Actor>& actor);								// Actor를 추가하는 함수
	void EreaseActor(const std::shared_ptr<class Actor>& actor);							// Actor를 제거하는 함수
	const std::list<std::shared_ptr<class Actor>>& GetActors() const { return actors; }

	std::weak_ptr<Scene> GetScene() { return std::shared_ptr<Scene>(this); }	// 현재 Scene을 반환함

	void MoveMap() { is_update = false; }

protected:
	class Context* context = nullptr;
	class Renderer* renderer = nullptr;

	bool is_update = true;					// 같은 actor일 경우에만 Render해주는 변수

	std::list<std::shared_ptr<class Actor>> actors;
	std::shared_ptr<class Actor> camera;
};