#include "stdafx.h"
#include "ScoreUI.h"

#include "Scene/Scene.h"
#include "Scene/Actor.h"

#include "Scene/Component/MeshRendererComponent.h"
#include "Scene/Component/MoveComponent/TransformComponent.h"
#include "Scene/Component/AnimationComponent/AnimatorComponent.h"

bool ScoreUI::Initialize()
{
    // 게임을 시작할 때는 0점부터 시작
    score = 0;
    
    return true;
}

// 현재 점수가 몇자리인지 알려줌
int CountDecimal(unsigned long long score)
{
    int count = 0;

    while (score / 10 != 0)
    {
        score /= 10;
        count++;
    }

    return count;
}

void ScoreUI::Update()
{
    const auto& input = context->GetSubSystem<InputManager>();

    if (!firstRun)
    {
        // 현재 씬 및 씬의 객체 목록 가져옴
        const auto& scene = context->GetSubSystem<SceneManager>()->GetCurrentScene();
        const auto& actors = context->GetSubSystem<SceneManager>()->GetCurrentScene()->GetActors();

        // UI를 붙여놓을 객체 생성
        UIActor.emplace(std::pair<std::string, std::shared_ptr<Actor>>("Score : ", scene->CreateActor()));
        UIActor.emplace(std::pair<std::string, std::shared_ptr<Actor>>("0", scene->CreateActor()));
        UIActor.emplace(std::pair<std::string, std::shared_ptr<Actor>>("1", scene->CreateActor()));
        UIActor.emplace(std::pair<std::string, std::shared_ptr<Actor>>("2", scene->CreateActor()));
        UIActor.emplace(std::pair<std::string, std::shared_ptr<Actor>>("3", scene->CreateActor()));
        UIActor.emplace(std::pair<std::string, std::shared_ptr<Actor>>("4", scene->CreateActor()));
        UIActor.emplace(std::pair<std::string, std::shared_ptr<Actor>>("5", scene->CreateActor()));
        UIActor.emplace(std::pair<std::string, std::shared_ptr<Actor>>("6", scene->CreateActor()));
        UIActor.emplace(std::pair<std::string, std::shared_ptr<Actor>>("7", scene->CreateActor()));
        UIActor.emplace(std::pair<std::string, std::shared_ptr<Actor>>("8", scene->CreateActor()));
        UIActor.emplace(std::pair<std::string, std::shared_ptr<Actor>>("9", scene->CreateActor()));

        // 해당 UI가 시작하는 씬의 객체 인덱스를 찾음
        auto actorIter = std::find(actors.begin(), actors.end(), UIActor.at("Score : "));

        // 새로 만든 객체를 순환함
        for (const auto& key : UIActor)
        {
            (*actorIter)->AddComponent<MeshRendererComponent>();
            (*actorIter)->AddComponent<TransformComponent>();

            // 모든 객체를 MainCamera의 부모로 설정함
            for (const auto& actor : actors)
            {
                if (actor->GetName() == "MainCamera")
                {
                    actor->GetComponent<TransformComponent>()->AddChild((*actorIter)->GetComponent<TransformComponent>());
                    (*actorIter)->GetComponent<TransformComponent>()->SetParent(actor->GetComponent<TransformComponent>());
                }
            }

            auto& animation = (*actorIter)->AddComponent<AnimatorComponent>();
            animation->SetAnimationMode(AnimationMode::Play);

            actorIter++;
        }

        // Score 라벨 위치, 크기, 이미지 설정
        UIActor["Score : "]->GetComponent<TransformComponent>()->SetLocalPosition(D3DXVECTOR3(-525.0f, 400.0f, -3.0f));
        UIActor["Score : "]->GetComponent<TransformComponent>()->SetLocalScale(D3DXVECTOR3(75.0f, 25.0f, 1.0f));
        UIActor["Score : "]->GetComponent<AnimatorComponent>()->SetAnimationFrameName(1, "label");

        // 각 자리수 위치, 크기, 이미지 설정
        for (int i = 0; i < 10; i++)
        {
            UIActor[std::to_string(i)]->GetComponent<TransformComponent>()->SetLocalPosition(D3DXVECTOR3(-455.0f + i * 25.0f, 400.0f, -3.0f));
            UIActor[std::to_string(i)]->GetComponent<TransformComponent>()->SetLocalScale(D3DXVECTOR3(25.0f, 25.0f, 1.0f));
            UIActor[std::to_string(i)]->GetComponent<AnimatorComponent>()->SetAnimationFrameName(11, "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "None");
        }

        // UI의 이미지를 가져올 XML 파일을 가져옴
        for (const auto& ui : UIActor)
        {
            if (ui.first.size() == 8)
                ui.second->GetComponent<AnimatorComponent>()->AddAnimation("Assets/Animation/UIScoreLabel.xml");

            else
                ui.second->GetComponent<AnimatorComponent>()->AddAnimation("Assets/Animation/Text.xml");

            ui.second->SetName("ScoreUI" + ui.first);
        }

        // Score 라벨은 XML파일의 label 애니메이션을 실행함
        UIActor["Score : "]->GetComponent<AnimatorComponent>()->SetCurrentAnimation("label");

        // 모든 자리수는 표시하지 않도록 설정
        for (int i = 0; i < 10; i++)
            UIActor[std::to_string(i)]->GetComponent<AnimatorComponent>()->SetCurrentAnimation("None");

        firstRun = true;
    }

    unsigned long long temp = score;    // 현재 점수를 가져옴
    int decimal = CountDecimal(temp);   // 점수가 몇자리인지 가져옴

    // 각 자리수마다 숫자를 표시
    for (int i = decimal; i >= 0; i--)
    {  
        UIActor[std::to_string(i)]->GetComponent<AnimatorComponent>()->SetCurrentAnimation(std::to_string(temp % 10));
        temp /= 10;
    }

    // 그 이상의 자리는 표시하지 않음
    for (int i = decimal + 1; i < 10; i++)
        if (UIActor[std::to_string(i)]->GetComponent<AnimatorComponent>()->GetNowKeyFrameName().compare("None"))
            UIActor[std::to_string(i)]->GetComponent<AnimatorComponent>()->SetCurrentAnimation("None");
}