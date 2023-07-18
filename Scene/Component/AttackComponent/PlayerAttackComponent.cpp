#include "stdafx.h"
#include "PlayerAttackComponent.h"

#include "Scene/Scene.h"

#include "../../Actor.h"
#include "../AnimationComponent/AnimatorComponent.h"
#include "../AnimationComponent/GaugeBarComponent.h"
#include "../AttackComponent/MultipleAttackTypesComponent.h"
#include "../MoveComponent/CollisionComponent.h"
#include "../MoveComponent/TransformComponent.h"
#include "../MeshRendererComponent.h"

#define HOLDPRESSKEYBOARD(input, key) (input->GetKeyState(key) == KEY_HOLD || input->GetKeyState(key) == KEY_DOWN)
#define ONLYPRESSKEYBOARD(input, key) (input->GetKeyState(key) == KEY_DOWN)

#define HOLDPRESSMOUSE(input, key) (input->GetMouseState(key) == KEY_HOLD || input->GetMouseState(key) == KEY_DOWN)
#define ONLYPRESSMOUSE(input, key) (input->GetMouseState(key) == KEY_DOWN)

void PlayerAttackComponent::Initialize()
{
	D3DXVECTOR3 scale = actor->GetComponent<TransformComponent>()->GetLocalScale();

	// 검 공격 스프라이트의 자식 클래스 생성
	sword.emplace(std::string("Front"), actor->GetScene()->CreateActor());
	sword.emplace(std::string("Back"), actor->GetScene()->CreateActor());
	sword.emplace(std::string("Left"), actor->GetScene()->CreateActor());
	sword.emplace(std::string("Right"), actor->GetScene()->CreateActor());

	for (const auto& child : sword)
	{
		child.second->SetActive(false);

		child.second->AddComponent<MeshRendererComponent>();
		child.second->AddComponent<TransformComponent>();
		child.second->AddComponent<CollisionComponent>();

		// 검 공격 스프라이트를 플레이어 객체에 부모로 설정 및 역과정 설정
		child.second->GetComponent<TransformComponent>()->SetParent(actor->GetComponent<TransformComponent>());
		actor->GetComponent<TransformComponent>()->AddChild(child.second->GetComponent<TransformComponent>());

		// 각각의 객체에 텍스쳐 부착
		auto& texture = child.second->AddComponent<AnimatorComponent>();
		texture->SetAnimationFrameName(1, (child.first + "Attack").c_str());
		texture->AddAnimation("Assets/Animation/Player/PlayerAttack.xml");
		child.second->SetName((child.first + "Attack").c_str());

		texture->SetCurrentAnimation(child.first + "Attack");
		texture->SetAnimationMode(AnimationMode::Stop);
	}

	sword.at("Front")->GetComponent<TransformComponent>()->SetLocalPosition(D3DXVECTOR3(0, 0.6, 0));
	sword.at("Front")->GetComponent<TransformComponent>()->SetScale(D3DXVECTOR3(scale.x * 3, scale.y, 1));

	sword.at("Back")->GetComponent<TransformComponent>()->SetLocalPosition(D3DXVECTOR3(0, -0.6, 0));
	sword.at("Back")->GetComponent<TransformComponent>()->SetScale(D3DXVECTOR3(scale.x * 3, scale.y, 1));

	sword.at("Left")->GetComponent<TransformComponent>()->SetLocalPosition(D3DXVECTOR3(-0.75, 0, 0));
	sword.at("Left")->GetComponent<TransformComponent>()->SetScale(D3DXVECTOR3(scale.x * 1.2, scale.y * 3, 1));

	sword.at("Right")->GetComponent<TransformComponent>()->SetLocalPosition(D3DXVECTOR3(0.75, 0, 0));
	sword.at("Right")->GetComponent<TransformComponent>()->SetScale(D3DXVECTOR3(scale.x * 1.2, scale.y * 3, 1));

	// 총 스프라이트 생성
	gun = actor->GetScene()->CreateActor();

	gun->AddComponent<MeshRendererComponent>();
	gun->AddComponent<TransformComponent>();
	gun->AddComponent<CollisionComponent>();

	gun->GetComponent<TransformComponent>()->SetParent(actor->GetComponent<TransformComponent>());
	actor->GetComponent<TransformComponent>()->AddChild(gun->GetComponent<TransformComponent>());

	auto& texture = gun->AddComponent<AnimatorComponent>();
	texture->SetAnimationFrameName(1, "Gun");
	texture->AddAnimation("Assets/Animation/Player/PlayerGun.xml");
	gun->SetName("PlayerGun");

	texture->SetCurrentAnimation("Gun");
	texture->SetAnimationMode(AnimationMode::Stop);

	gun->GetComponent<TransformComponent>()->SetLocalPosition(D3DXVECTOR3(0, 0.7, 0));
	gun->GetComponent<TransformComponent>()->SetScale(D3DXVECTOR3(scale.x / 4, scale.x / 4, 1));

	gun->SetActive(false);

	// 피격 스프라이트의 자식 클래스 생성
	shot.emplace(std::string("RightShort"), actor->GetScene()->CreateActor());
	shot.emplace(std::string("LeftShort"), actor->GetScene()->CreateActor());
	shot.emplace(std::string("FrontShort"), actor->GetScene()->CreateActor());
	shot.emplace(std::string("BackShort"), actor->GetScene()->CreateActor());

	for (const auto& child : shot)
	{
		child.second->SetActive(false);

		child.second->AddComponent<CollisionComponent>();
		child.second->AddComponent<MeshRendererComponent>();
		child.second->AddComponent<TransformComponent>();
		child.second->AddComponent<MultipleAttackTypesComponent>();

		// 체력 바의 자식 클래스 체력 바 게이지 객체에 부모로 설정 및 역과정 설정
		child.second->GetComponent<TransformComponent>()->SetParent(actor->GetComponent<TransformComponent>());
		actor->GetComponent<TransformComponent>()->AddChild(child.second->GetComponent<TransformComponent>());

		// 각각의 객체에 텍스쳐 부착
		auto& texture = child.second->AddComponent<AnimatorComponent>();
		texture->SetAnimationFrameName(1, (child.first + "Shot").c_str());
		texture->AddAnimation("Assets/Animation/Player/PlayerShortShot.xml");
		child.second->SetName((child.first + "Shot").c_str());

		texture->SetCurrentAnimation(child.first + "Shot");
		texture->SetAnimationMode(AnimationMode::Stop);

		child.second->GetComponent<MultipleAttackTypesComponent>()->SetAttackType(AttackType::Short);

		// 총알 스프라이트가 모든 몬스터가 충돌할 수 있도록 충돌타겟 설정
		for (const auto& object : actor->GetScene()->GetActors())
		{
			if (object->GetName().size() < 7) continue;

			if (object->GetName().substr(object->GetName().size() - 7, 7) == "Monster")
			{
				child.second->GetComponent<CollisionComponent>()->SetCollistionTarget(object);
				child.second->GetComponent<CollisionComponent>()->SetCollisionArea(StartArea::Top, 1.0f);
			}
		}
	}

	shot.at("FrontShort")->GetComponent<TransformComponent>()->SetLocalPosition(D3DXVECTOR3(0, 1.5, 0));
	shot.at("FrontShort")->GetComponent<TransformComponent>()->SetLocalScale(D3DXVECTOR3(2.5, 2.5, 1));

	shot.at("BackShort")->GetComponent<TransformComponent>()->SetLocalPosition(D3DXVECTOR3(0, -1.5, 0));
	shot.at("BackShort")->GetComponent<TransformComponent>()->SetLocalScale(D3DXVECTOR3(2.5, 2.5, 1));
	
	shot.at("LeftShort")->GetComponent<TransformComponent>()->SetLocalPosition(D3DXVECTOR3(-1.5, 0, 0));
	shot.at("LeftShort")->GetComponent<TransformComponent>()->SetLocalScale(D3DXVECTOR3(2.5, 2.5, 1));
	
	shot.at("RightShort")->GetComponent<TransformComponent>()->SetLocalPosition(D3DXVECTOR3(1.5, 0, 0));
	shot.at("RightShort")->GetComponent<TransformComponent>()->SetLocalScale(D3DXVECTOR3(2.5, 2.5, 1));

	timer = context->GetSubSystem<Timer>();
}

POINT GetCursorSystem::mouse;

void PlayerAttackComponent::Update()
{
	auto input = context->GetSubSystem<InputManager>();

	if((bulletStyle == PlayerBulletStyle::Long) && !gun->IsActive()) gun->SetActive(true);
	else if(bulletStyle == PlayerBulletStyle::Short) gun->SetActive(false);

	POINT curPos = GetCursorSystem::mouse;							// 마우스 위치
	float curDiff = sqrt(pow(curPos.x, 2) + pow(curPos.y, 2));		// 마우스와 플레이어 위치차이

	// 플레이어 총구 회전
	if (gun->IsActive())
	{
		if (curDiff != 0)
		{
			gun->GetComponent<TransformComponent>()->SetLocalRotation(D3DXVECTOR3(0, 0, atan2(curPos.y / curDiff, curPos.x / curDiff)));
			gun->GetComponent<TransformComponent>()->SetLocalPosition(D3DXVECTOR3(curPos.x / curDiff, curPos.y / curDiff, 0));
		}
	}

	if (nowDelay <= 0 && (HOLDPRESSMOUSE(input, MOUSE_LEFT)))
	{
		if (bulletStyle == PlayerBulletStyle::Short)
		{
			if (gun->IsActive()) gun->SetActive(false);

			switch (actor->GetDirection())
			{
			case Direction::Front:
				sword.at("Front")->SetActive(true);
				shot.at("FrontShort")->SetActive(true);

				sword.at("Front")->GetComponent<AnimatorComponent>()->Play();
				shot.at("FrontShort")->GetComponent<AnimatorComponent>()->Play();
				break;

			case Direction::Back:
				sword.at("Back")->SetActive(true);
				shot.at("BackShort")->SetActive(true);

				sword.at("Back")->GetComponent<AnimatorComponent>()->Play();
				shot.at("BackShort")->GetComponent<AnimatorComponent>()->Play();
				break;

			case Direction::Left:
				sword.at("Left")->SetActive(true);
				shot.at("LeftShort")->SetActive(true);

				sword.at("Left")->GetComponent<AnimatorComponent>()->Play();
				shot.at("LeftShort")->GetComponent<AnimatorComponent>()->Play();
				break;

			case Direction::Right:
				sword.at("Right")->SetActive(true);
				shot.at("RightShort")->SetActive(true);

				sword.at("Right")->GetComponent<AnimatorComponent>()->Play();
				shot.at("RightShort")->GetComponent<AnimatorComponent>()->Play();
				break;

			default:
				break;
			}
		}

		else if (bulletStyle == PlayerBulletStyle::Long)
		{
			const auto& gunTransform = gun->GetComponent<TransformComponent>()->GetPosition();

			bullet.emplace_back(actor->GetScene()->CreateActor());

			bullet.back()->AddComponent<MeshRendererComponent>();
			bullet.back()->AddComponent<TransformComponent>();
			bullet.back()->AddComponent<CollisionComponent>();

			bullet.back()->GetComponent<TransformComponent>()->SetParent(actor->GetComponent<TransformComponent>());
			actor->GetComponent<TransformComponent>()->AddChild(bullet.back()->GetComponent<TransformComponent>());

			auto texture = bullet.back()->AddComponent<AnimatorComponent>();
			texture->SetAnimationFrameName(1, "Bullet");
			texture->AddAnimation("Assets/Animation/Bullet/PlayerBulletShot.xml");
			bullet.back()->SetName("Bullet");

			texture->SetCurrentAnimation("Bullet");
			texture->SetAnimationMode(AnimationMode::Stop);

			bullet.back()->GetComponent<TransformComponent>()->SetPosition(D3DXVECTOR3(gunTransform.x, gunTransform.y, 0));
			bullet.back()->GetComponent<TransformComponent>()->SetLocalScale(D3DXVECTOR3(0.2f, 0.2f, 1.0f));
			bullet.back()->GetComponent<TransformComponent>()->SetLocalRotation(D3DXVECTOR3(0, 0, atan2(curPos.y / curDiff, curPos.x / curDiff) - PI / 2));
			bullet.back()->GetComponent<TransformComponent>()->SetFront(D3DXVECTOR3(curPos.x / curDiff, curPos.y / curDiff, 0));

			// 총알 스프라이트가 모든 몬스터가 충돌할 수 있도록 충돌타겟 설정
			for (const auto& object : actor->GetScene()->GetActors())
			{
				if (object->GetName().size() < 7) continue;

				if (object->GetName().substr(object->GetName().size() - 7, 7) == "Monster")
				{
					bullet.back()->GetComponent<CollisionComponent>()->SetCollistionTarget(object);
					bullet.back()->GetComponent<CollisionComponent>()->SetCollisionArea(StartArea::Top, 1.0f);
				}
			}
		}

		isAttack = true;
		isDamage = false;
		nowDelay = delay;
	}

	else nowDelay -= timer->GetDeltaTimeSEC();

	// 시간을 측정하여 화면 출력 시간이 끝난 경우 비활성화
	if (animationPlay <= 0)
	{
		for (const auto& dir : sword)
		{
			dir.second->GetComponent<AnimatorComponent>()->Stop();
			dir.second->SetActive(false);
		}

		for (const auto& dir : shot)
		{
			dir.second->GetComponent<AnimatorComponent>()->Stop();
			dir.second->SetActive(false);
		}

		animationPlay = 0.35f;
		isAttack = false;
	}

	else if (isAttack) animationPlay -= timer->GetDeltaTimeSEC();

	// 플레이어의 피격 스프라이트와 충돌한 몬스터 데미지 처리
	for (const auto& attack : shot)
	{
		if (!(attack.second->GetComponent<CollisionComponent>()->IsCollision())) continue;
		if (isDamage || !attack.second->IsActive()) continue;

		// 피격 스프라이트가 다른 물체와 충돌했을 경우
		const auto& collision = attack.second->GetComponent<CollisionComponent>()->GetCollisionObject();

		for (auto monster = collision.begin(); monster != collision.end();)
		{
			if (monster->expired())
			{
				monster++;
				continue;
			}

			for (const auto& child : (*monster).lock()->GetComponent<TransformComponent>()->GetChilds())
			{
				if (child.expired() || child.lock()->GetActor()->GetName().size() < 8)
					continue;

				// 게이지바가 붙은 객체에 데미지를 줌
				if (child.lock()->GetActor()->GetName().substr(0, 8) == "GaugeBar")
					child.lock()->GetActor()->GetComponent<GaugeBarComponent>()->SetDamegeHp(damage);
			}

			if ((++monster) == collision.end())
				isDamage = true;
		}

		if (attack.second->GetComponent<MultipleAttackTypesComponent>()->GetAttackType() == AttackType::Long)
			shot.erase(attack.first);
	}

	// 총알 이동 처리
	for (const auto& bul : bullet)
	{
		const auto& bulletTransform = bul->GetComponent<TransformComponent>();
	
		bulletTransform->SetPosition(bulletTransform->GetPosition() + bulletTransform->GetFront() * bulletSpeed);
	}

	// 범위를 벗어난 총알 제거
	for (auto iter = bullet.begin(); iter != bullet.end();)
	{
		if ((*iter)->GetComponent<TransformComponent>()->GetPosition().x > WorldX / 2 || (*iter)->GetComponent<TransformComponent>()->GetPosition().x < -WorldX / 2 ||
			(*iter)->GetComponent<TransformComponent>()->GetPosition().y > WorldY / 2 || (*iter)->GetComponent<TransformComponent>()->GetPosition().y < -WorldY / 2)
		{
			(*iter)->Destroy();
			(*iter)->GetScene()->EreaseActor((*iter));

			iter = bullet.erase(iter);
		}

		else iter++;
	}

	// 플레이어의 총알 스프라이트와 충돌한 몬스터 데미지 처리
	for (auto bull = bullet.begin(); bull != bullet.end();)
	{
		if (!((*bull)->GetComponent<CollisionComponent>()->IsCollision()))
		{
			bull++;
			continue;
		}

		if (isDamage && !(*bull)->IsActive())
		{
			bull++;
			continue;
		}

		// 피격 스프라이트가 다른 물체와 충돌했을 경우
		const auto& collision = (*bull)->GetComponent<CollisionComponent>()->GetCollisionObject();

		for (auto monster = collision.begin(); monster != collision.end();)
		{
			if (monster->expired())
			{
				monster++;
				continue;
			}

			for (const auto& child : (*monster).lock()->GetComponent<TransformComponent>()->GetChilds())
			{
				if (child.expired() || child.lock()->GetActor()->GetName().size() < 8)
					continue;

				// 게이지바가 붙은 객체에 데미지를 줌
				if (child.lock()->GetActor()->GetName().substr(0, 8) == "GaugeBar")
				{
					child.lock()->GetActor()->GetComponent<GaugeBarComponent>()->SetDamegeHp(damage);

					// 총알 삭제
					if (bulletStyle == PlayerBulletStyle::Long)
					{
						(*bull)->Destroy();
						(*bull)->GetScene()->EreaseActor(*bull);
						bull = bullet.erase(std::find(bullet.begin(), bullet.end(), *bull));
						isDamage = true;
						break;
					}
				}
			}

			if (isDamage) break;

			monster++;
		}

		if (isDamage) break;

		bull++;
	}
}

void PlayerAttackComponent::Destroy()
{
	
}