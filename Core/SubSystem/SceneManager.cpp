#include "stdafx.h"
#include "SceneManager.h"
#include "Scene/Scene.h"

bool SceneManager::Initialize()
{
    RegisterScene("First");
    SetCurrentScene("First");

    return true;
}

void SceneManager::Update()
{
    if (current_scene.expired() != true)
        current_scene.lock()->Update();
}

Scene* const SceneManager::GetCurrentScene()
{
    return current_scene.expired() ? nullptr : current_scene.lock().get();
}

void SceneManager::SetCurrentScene(const std::string& scene_name)
{
    // 해당 값이 Scenes에 있는지 확인
    if (scenes.find(scene_name) == scenes.end())
    {
        assert(false);
        return;
    }

    current_scene = scenes[scene_name];
}

Scene* const SceneManager::RegisterScene(const std::string& scene_name)
{
    // 만약 입력한 Scene이 이미 Scene에 있는지 확인
    if (scenes.find(scene_name) != scenes.end())
    {
        assert(false);
        return nullptr;
    }

    std::shared_ptr<Scene> new_scene = std::make_shared<Scene>(context);

    scenes[scene_name] = new_scene;

    return new_scene.get();
}
