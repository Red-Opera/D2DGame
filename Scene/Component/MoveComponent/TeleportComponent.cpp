#include "stdafx.h"
#include "TeleportComponent.h"
#include "TransformComponent.h"

#include "../../Actor.h"
#include "../../Scene.h"
#include "../MoveComponent/CollisionComponent.h"

void TeleportComponent::Initialize()
{
	// �ش� ������Ʈ�� ����ϴ� Actor ã��
	parent = GetActor();

	// ���� Actor�� ���� ������
	scene = actor->GetScene();

	// �浹 �ν� ������Ʈ �߰�
	collsion = parent->AddComponent<CollisionComponent>();

	for (auto& act : scene->GetActors())
		if (act->GetName() == "Player")
			player = act;

	collsion.lock()->SetCollistionTarget(player);
	collsion.lock()->SetCollisionArea(StartArea::Bottom, 1.0f);
}

void TeleportComponent::Update()
{
	// ���� �÷��̾�� �浹�� ���
	if (collsion.lock()->GetCollisionObject().size())
	{
		// �̵��� ���� �����ϰ� �� ����
		context->GetSubSystem<SceneManager>()->RegisterScene(mapName);
		context->GetSubSystem<SceneManager>()->SetCurrentScene(mapName);

		// �� ������ ����
		scene->MoveMap();

		// �������ϴ� ī�޶� ����
		context->GetSubSystem<Renderer>()->SetCameraTarget(nullptr);
	}
}

void TeleportComponent::Destroy()
{

}