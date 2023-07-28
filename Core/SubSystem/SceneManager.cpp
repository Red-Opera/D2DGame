#include "stdafx.h"
#include "SceneManager.h"
#include "Scene/Scene.h"
#include "Scene/MainScene.h"
#include "Scene/WidePlainScene.h"

bool SceneManager::Initialize()
{
    RegisterScene("WidePlainScene");

    SetCurrentScene("WidePlainScene");

    return true;
}

void SceneManager::Update()
{
    if (current_scene.expired() != true)
    {
        if (!nowSceneName.compare("WidePlainScene"))
            std::static_pointer_cast<WidePlainScene>(current_scene.lock())->Update();

        else if (!nowSceneName.compare("MainScene"))
            std::static_pointer_cast<MainScene>(current_scene.lock())->Update();
    }
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
    nowSceneName = scene_name;
}

Scene* const SceneManager::RegisterScene(const std::string& scene_name)
{
    // 만약 입력한 Scene이 이미 Scene에 있는지 확인
    if (scenes.find(scene_name) != scenes.end())
    {
        assert(false);
        return nullptr;
    }

    std::shared_ptr<Scene> new_scene;
    if (scene_name == "WidePlainScene")
        new_scene = std::make_shared<WidePlainScene>(context);

    else if (scene_name == "MainScene")
        new_scene = std::make_shared<MainScene>(context);

    scenes[scene_name] = new_scene;

    return new_scene.get();
}
