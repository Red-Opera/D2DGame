// Actor.cpp

#include "stdafx.h"
#include "Actor.h"
#include "Scene.h"
#include "Component/MoveComponent/TransformComponent.h"
#include "Component/MeshRendererComponent.h"

Actor::Actor(class Context* const context) : context(context)
{
    // Component�� ��ġ�� ����
    AddComponent<TransformComponent>();
}

Actor::~Actor()
{
    
}

void Actor::Initialize()
{
    // ���� �ִ� Component��� �ʱ�ȭ
    for (const auto& component : components)
        component->Initialize();
}

void Actor::Update()
{
    if (!is_active) return;

    // ���� �ִ� Component��� ������Ʈ
    for (const auto& component : components)
        if(component->IsEnabled())
            component->Update();
}

void Actor::Destroy()
{
    // ���� �ִ� Component��� ����
    for (const auto& component : components)
        component->Destroy();
}

bool Actor::HasComponent(const ComponentType& type)
{
    // ��� ���ҿ��� �Է��� ComponentŸ���� �ִ��� Ȯ��
    for (const auto& component : components)
        if (component->GetComponentType() == type) return true;

    return false;
}
