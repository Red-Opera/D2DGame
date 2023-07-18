#include "stdafx.h"
#include "PressUI.h"
#include "Scene/Scene.h"
#include "Scene/Actor.h"
#include "Scene/Component/MeshRendererComponent.h"
#include "Scene/Component/MoveComponent/TransformComponent.h"
#include "Scene/Component/AnimationComponent/AnimatorComponent.h"

#define HOLDPRESSKEYBOARD(input, key) (input->GetKeyState(key) == KEY_HOLD || input->GetKeyState(key) == KEY_DOWN)
#define ONLYPRESSKEYBOARD(input, key) (input->GetKeyState(key) == KEY_DOWN)

#define HOLDPRESSMOUSE(input, key) (input->GetMouseState(key) == KEY_HOLD || input->GetMouseState(key) == KEY_DOWN)
#define ONLYPRESSMOUSE(input, key) (input->GetMouseState(key) == KEY_DOWN)

bool PressUI::Initialize()
{
    return false;
}

void PressUI::Update()
{
    const auto& input = context->GetSubSystem<InputManager>();

    if (!firstRun)
    {
        const auto& scene = context->GetSubSystem<SceneManager>()->GetCurrentScene();
        const auto& actors = context->GetSubSystem<SceneManager>()->GetCurrentScene()->GetActors();

        UIActor.emplace(std::pair<std::string, std::shared_ptr<Actor>>("W", scene->CreateActor()));
        UIActor.emplace(std::pair<std::string, std::shared_ptr<Actor>>("A", scene->CreateActor()));
        UIActor.emplace(std::pair<std::string, std::shared_ptr<Actor>>("S", scene->CreateActor()));
        UIActor.emplace(std::pair<std::string, std::shared_ptr<Actor>>("D", scene->CreateActor()));
        UIActor.emplace(std::pair<std::string, std::shared_ptr<Actor>>("C", scene->CreateActor()));
        UIActor.emplace(std::pair<std::string, std::shared_ptr<Actor>>("1", scene->CreateActor()));
        UIActor.emplace(std::pair<std::string, std::shared_ptr<Actor>>("2", scene->CreateActor()));
        UIActor.emplace(std::pair<std::string, std::shared_ptr<Actor>>("LeftMouse", scene->CreateActor()));

        auto actorIter = std::find(actors.begin(), actors.end(), UIActor.at("W"));

        // AddComponent
        for (const auto& key : UIActor)
        {
            (*actorIter)->AddComponent<MeshRendererComponent>();
            (*actorIter)->AddComponent<TransformComponent>();

            for (const auto& actor : actors)
            {
                if (actor->GetName() == "MainCamera")
                {
                    actor->GetComponent<TransformComponent>()->AddChild((*actorIter)->GetComponent<TransformComponent>());
                    (*actorIter)->GetComponent<TransformComponent>()->SetParent(actor->GetComponent<TransformComponent>());
                }
            }

            (*actorIter)->GetComponent<TransformComponent>()->SetScale(D3DXVECTOR3(50.0f, 50.0f, 1.0f));

            auto& animation = (*actorIter)->AddComponent<AnimatorComponent>();
            animation->SetAnimationMode(AnimationMode::Play);

            actorIter++;
        }

        UIActor.at("W")->GetComponent<TransformComponent>()->SetLocalPosition(D3DXVECTOR3(-500.0f, -350.0f, -3.0f));
        UIActor.at("A")->GetComponent<TransformComponent>()->SetLocalPosition(D3DXVECTOR3(-550.0f, -400.0f, -3.0f));
        UIActor.at("S")->GetComponent<TransformComponent>()->SetLocalPosition(D3DXVECTOR3(-500.0f, -400.0f, -3.0f));
        UIActor.at("D")->GetComponent<TransformComponent>()->SetLocalPosition(D3DXVECTOR3(-450.0f, -400.0f, -3.0f));
        UIActor.at("C")->GetComponent<TransformComponent>()->SetLocalPosition(D3DXVECTOR3(-350.0f, -400.0f, -3.0f));
        UIActor.at("1")->GetComponent<TransformComponent>()->SetLocalPosition(D3DXVECTOR3(-550.0f, -275.0f, -3.0f));
        UIActor.at("2")->GetComponent<TransformComponent>()->SetLocalPosition(D3DXVECTOR3(-500.0f, -275.0f, -3.0f));
        UIActor.at("LeftMouse")->GetComponent<TransformComponent>()->SetLocalPosition(D3DXVECTOR3(-375.0f, -300.0f, -3.0f));

        UIActor.at("W")->GetComponent<AnimatorComponent>()->SetAnimationFrameName(2, "NW", "YW");
        UIActor.at("A")->GetComponent<AnimatorComponent>()->SetAnimationFrameName(2, "NA", "YA");
        UIActor.at("S")->GetComponent<AnimatorComponent>()->SetAnimationFrameName(2, "NS", "YS");
        UIActor.at("D")->GetComponent<AnimatorComponent>()->SetAnimationFrameName(2, "ND", "YD");
        UIActor.at("C")->GetComponent<AnimatorComponent>()->SetAnimationFrameName(2, "NC", "YC");
        UIActor.at("1")->GetComponent<AnimatorComponent>()->SetAnimationFrameName(2, "N1", "Y1");
        UIActor.at("2")->GetComponent<AnimatorComponent>()->SetAnimationFrameName(2, "N2", "Y2");
        UIActor.at("LeftMouse")->GetComponent<AnimatorComponent>()->SetAnimationFrameName(2, "Press", "NoPress");

        for (const auto& ui : UIActor)
        {
            if (ui.first.size() == 1)
                ui.second->GetComponent<AnimatorComponent>()->AddAnimation("Assets/Animation/UIKeyBoard.xml");
            
            else
                ui.second->GetComponent<AnimatorComponent>()->AddAnimation("Assets/Animation/UILeftMouse.xml");
               
            ui.second->SetName("KeyBoardUI" + ui.first);
        }

        UIActor.at("W")->GetComponent<AnimatorComponent>()->SetCurrentAnimation("NW");
        UIActor.at("A")->GetComponent<AnimatorComponent>()->SetCurrentAnimation("NA");
        UIActor.at("S")->GetComponent<AnimatorComponent>()->SetCurrentAnimation("NS");
        UIActor.at("D")->GetComponent<AnimatorComponent>()->SetCurrentAnimation("ND");
        UIActor.at("C")->GetComponent<AnimatorComponent>()->SetCurrentAnimation("NC");
        UIActor.at("1")->GetComponent<AnimatorComponent>()->SetCurrentAnimation("N1");
        UIActor.at("2")->GetComponent<AnimatorComponent>()->SetCurrentAnimation("N2");
        UIActor.at("LeftMouse")->GetComponent<AnimatorComponent>()->SetCurrentAnimation("NoPress");
        
        firstRun = true;
    }

    if (HOLDPRESSKEYBOARD(input, DIK_W))
        UIActor.at("W")->GetComponent<AnimatorComponent>()->SetCurrentAnimation("YW");

    else UIActor.at("W")->GetComponent<AnimatorComponent>()->SetCurrentAnimation("NW");

    if (HOLDPRESSKEYBOARD(input, DIK_A))
        UIActor.at("A")->GetComponent<AnimatorComponent>()->SetCurrentAnimation("YA");

    else UIActor.at("A")->GetComponent<AnimatorComponent>()->SetCurrentAnimation("NA");

    if (HOLDPRESSKEYBOARD(input, DIK_S))
        UIActor.at("S")->GetComponent<AnimatorComponent>()->SetCurrentAnimation("YS");

    else UIActor.at("S")->GetComponent<AnimatorComponent>()->SetCurrentAnimation("NS");


    if (HOLDPRESSKEYBOARD(input, DIK_D))
        UIActor.at("D")->GetComponent<AnimatorComponent>()->SetCurrentAnimation("YD");

    else UIActor.at("D")->GetComponent<AnimatorComponent>()->SetCurrentAnimation("ND");

    if (HOLDPRESSKEYBOARD(input, DIK_C))
        UIActor.at("C")->GetComponent<AnimatorComponent>()->SetCurrentAnimation("YC");

    else UIActor.at("C")->GetComponent<AnimatorComponent>()->SetCurrentAnimation("NC");

    if (HOLDPRESSKEYBOARD(input, DIK_1))
        UIActor.at("1")->GetComponent<AnimatorComponent>()->SetCurrentAnimation("Y1");

    else UIActor.at("1")->GetComponent<AnimatorComponent>()->SetCurrentAnimation("N1");

    if (HOLDPRESSKEYBOARD(input, DIK_2))
        UIActor.at("2")->GetComponent<AnimatorComponent>()->SetCurrentAnimation("Y2");

    else UIActor.at("2")->GetComponent<AnimatorComponent>()->SetCurrentAnimation("N2");

    if (HOLDPRESSMOUSE(input, MOUSE_LEFT))
        UIActor.at("LeftMouse")->GetComponent<AnimatorComponent>()->SetCurrentAnimation("Press");

    else UIActor.at("LeftMouse")->GetComponent<AnimatorComponent>()->SetCurrentAnimation("NoPress");
}
