// Scene.h

#pragma once

class Scene final
{
public:
	Scene(class Context* const context);
	~Scene();
	
	void Update();
	
	const std::shared_ptr<class Actor> CreateActor(const bool& is_active = true);			// Actor�� �߰��ϰ� �ٷ� Ȱ��ȭ�ϴ� �Լ�
	void AddActor(const std::shared_ptr<class Actor>& actor);								// Actor�� �߰��ϴ� �Լ�
	void EreaseActor(const std::shared_ptr<class Actor>& actor);							// Actor�� �����ϴ� �Լ�
	const std::list<std::shared_ptr<class Actor>>& GetActors() const { return actors; }

	std::weak_ptr<Scene> GetScene() { return std::shared_ptr<Scene>(this); }	// ���� Scene�� ��ȯ��

private:
	class Context* context = nullptr;
	class Renderer* renderer = nullptr;

	bool is_update = true;					// ���� actor�� ��쿡�� Render���ִ� ����

	Timer* timer = 0;						// Ÿ�Ӹ�
	float nowDelay = 0;						// ���� ���� �������� ���� �ð�
	unsigned int onMonster = 0;				// Ȱ����Ų ���� ��

	std::list<std::shared_ptr<class Actor>> actors;
};