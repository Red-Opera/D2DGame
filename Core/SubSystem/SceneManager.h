#pragma once
#include "ISubSystem.h"

class SceneManager final : public ISubSystem
{
public:
	using ISubSystem::ISubSystem;
	~SceneManager() = default;

	// ISubSystem��(��) ���� ��ӵ�
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
		Vector ����
			- ���� �迭
			- ���� ���� ����(���� ������ ����)
			- �����͸� ���������� ó���ϴ°� ����

		Vector ����
			- �߰� ���ҿ� ���� ����/���� �Ҹ�
			- ũ�Ⱑ ������ �ȵ� ��� ���Ҵ��� ���� �߻���
			- ���ĵ��� �ʴ� ���¿����� Ž���� �Ҹ���
	*/
};