#include "stdafx.h"
#include "ScoreUI.h"

#include "Scene/Scene.h"
#include "Scene/Actor.h"

#include "Scene/Component/MeshRendererComponent.h"
#include "Scene/Component/MoveComponent/TransformComponent.h"
#include "Scene/Component/AnimationComponent/AnimatorComponent.h"

bool ScoreUI::Initialize()
{
    // ������ ������ ���� 0������ ����
    score = 0;
    
    return true;
}

// ���� ������ ���ڸ����� �˷���
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
        // ���� �� �� ���� ��ü ��� ������
        const auto& scene = context->GetSubSystem<SceneManager>()->GetCurrentScene();
        const auto& actors = context->GetSubSystem<SceneManager>()->GetCurrentScene()->GetActors();

        // UI�� �ٿ����� ��ü ����
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

        // �ش� UI�� �����ϴ� ���� ��ü �ε����� ã��
        auto actorIter = std::find(actors.begin(), actors.end(), UIActor.at("Score : "));

        // ���� ���� ��ü�� ��ȯ��
        for (const auto& key : UIActor)
        {
            (*actorIter)->AddComponent<MeshRendererComponent>();
            (*actorIter)->AddComponent<TransformComponent>();

            // ��� ��ü�� MainCamera�� �θ�� ������
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

        // Score �� ��ġ, ũ��, �̹��� ����
        UIActor["Score : "]->GetComponent<TransformComponent>()->SetLocalPosition(D3DXVECTOR3(-525.0f, 400.0f, -3.0f));
        UIActor["Score : "]->GetComponent<TransformComponent>()->SetLocalScale(D3DXVECTOR3(75.0f, 25.0f, 1.0f));
        UIActor["Score : "]->GetComponent<AnimatorComponent>()->SetAnimationFrameName(1, "label");

        // �� �ڸ��� ��ġ, ũ��, �̹��� ����
        for (int i = 0; i < 10; i++)
        {
            UIActor[std::to_string(i)]->GetComponent<TransformComponent>()->SetLocalPosition(D3DXVECTOR3(-455.0f + i * 25.0f, 400.0f, -3.0f));
            UIActor[std::to_string(i)]->GetComponent<TransformComponent>()->SetLocalScale(D3DXVECTOR3(25.0f, 25.0f, 1.0f));
            UIActor[std::to_string(i)]->GetComponent<AnimatorComponent>()->SetAnimationFrameName(11, "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "None");
        }

        // UI�� �̹����� ������ XML ������ ������
        for (const auto& ui : UIActor)
        {
            if (ui.first.size() == 8)
                ui.second->GetComponent<AnimatorComponent>()->AddAnimation("Assets/Animation/UIScoreLabel.xml");

            else
                ui.second->GetComponent<AnimatorComponent>()->AddAnimation("Assets/Animation/Text.xml");

            ui.second->SetName("ScoreUI" + ui.first);
        }

        // Score ���� XML������ label �ִϸ��̼��� ������
        UIActor["Score : "]->GetComponent<AnimatorComponent>()->SetCurrentAnimation("label");

        // ��� �ڸ����� ǥ������ �ʵ��� ����
        for (int i = 0; i < 10; i++)
            UIActor[std::to_string(i)]->GetComponent<AnimatorComponent>()->SetCurrentAnimation("None");

        firstRun = true;
    }

    unsigned long long temp = score;    // ���� ������ ������
    int decimal = CountDecimal(temp);   // ������ ���ڸ����� ������

    // �� �ڸ������� ���ڸ� ǥ��
    for (int i = decimal; i >= 0; i--)
    {  
        UIActor[std::to_string(i)]->GetComponent<AnimatorComponent>()->SetCurrentAnimation(std::to_string(temp % 10));
        temp /= 10;
    }

    // �� �̻��� �ڸ��� ǥ������ ����
    for (int i = decimal + 1; i < 10; i++)
        if (UIActor[std::to_string(i)]->GetComponent<AnimatorComponent>()->GetNowKeyFrameName().compare("None"))
            UIActor[std::to_string(i)]->GetComponent<AnimatorComponent>()->SetCurrentAnimation("None");
}