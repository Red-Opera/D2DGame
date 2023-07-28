#include "stdafx.h"
#include "TeleportComponent.h"
#include "TransformComponent.h"

#include "../../Actor.h"
#include "../../Scene.h"
#include "../MoveComponent/CollisionComponent.h"

void TeleportComponent::Initialize()
{
	// 해당 컴포넌트를 사용하는 Actor 찾기
	parent = GetActor();

	// 현재 Actor의 씬을 가져옴
	scene = actor->GetScene();

	// 충돌 인식 컴포넌트 추가
	collsion = parent->AddComponent<CollisionComponent>();

	for (auto& act : scene->GetActors())
		if (act->GetName() == "Player")
			player = act;

	collsion.lock()->SetCollistionTarget(player);
	collsion.lock()->SetCollisionArea(StartArea::Bottom, 1.0f);
}

void TeleportComponent::Update()
{
	// 만약 플레이어와 충돌한 경우
	if (collsion.lock()->GetCollisionObject().size())
	{
		// 이동할 맵을 생성하고 맵 변경
		context->GetSubSystem<SceneManager>()->RegisterScene(mapName);
		context->GetSubSystem<SceneManager>()->SetCurrentScene(mapName);

		// 맵 렌더링 중지
		scene->MoveMap();

		// 렌더링하는 카메라 제거
		context->GetSubSystem<Renderer>()->SetCameraTarget(nullptr);
	}
}

void TeleportComponent::Destroy()
{

}