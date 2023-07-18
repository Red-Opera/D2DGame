// Actor.cpp

#include "stdafx.h"
#include "Actor.h"
#include "Scene.h"
#include "Component/MoveComponent/TransformComponent.h"
#include "Component/MeshRendererComponent.h"

Actor::Actor(class Context* const context) : context(context)
{
    // Component의 위치를 얻음
    AddComponent<TransformComponent>();
}

Actor::~Actor()
{
    
}

void Actor::Initialize()
{
    // 갖고 있는 Component모두 초기화
    for (const auto& component : components)
        component->Initialize();
}

void Actor::Update()
{
    if (!is_active) return;

    // 갖고 있는 Component모두 업데이트
    for (const auto& component : components)
        if(component->IsEnabled())
            component->Update();
}

void Actor::Destroy()
{
    // 갖고 있는 Component모두 삭제
    for (const auto& component : components)
        component->Destroy();
}

bool Actor::HasComponent(const ComponentType& type)
{
    // 모든 원소에서 입력한 Component타입이 있는지 확인
    for (const auto& component : components)
        if (component->GetComponentType() == type) return true;

    return false;
}
