#include "stdafx.h"
#include "GaugeBarComponent.h"
#include "AnimatorComponent.h"

#include "Scene/Scene.h"

#include "../MeshRendererComponent.h"
#include "../AttackComponent/BulletComponent.h"
#include "../MoveComponent/TransformComponent.h"

#include "../../Actor.h"

void GaugeBarComponent::Initialize()
{
	// actor는 게이지 바를 의미
	if (!actor->HasCompoent<TransformComponent>())
	{
		assert(false && "ERROR (GaugetBarComponent2) : 게이지 바에 TransformComponent가 없음");
		return;
	}

	// 체력 바의 자식 클래스 생성
	children.emplace(std::string("HpBar"), actor->GetScene()->CreateActor());
	children.emplace(std::string("ManaBar"), actor->GetScene()->CreateActor());
	children.emplace(std::string("HpGauge"), actor->GetScene()->CreateActor());
	children.emplace(std::string("ManaGauge"), actor->GetScene()->CreateActor());

	for (const auto& child : children)
	{
		child.second->AddComponent<MeshRendererComponent>();
		child.second->AddComponent<TransformComponent>();

		// 체력 바의 자식 클래스 체력 바 게이지 객체에 부모로 설정 및 역과정 설정
		child.second->GetComponent<TransformComponent>()->SetParent(actor->GetComponent<TransformComponent>());
		actor->GetComponent<TransformComponent>()->AddChild(child.second->GetComponent<TransformComponent>());

		// 각각의 객체에 텍스쳐 부착
		auto& texture = child.second->AddComponent<AnimatorComponent>();
		texture->SetAnimationFrameName(1, child.first.c_str());
		texture->AddAnimation("Assets/Animation/GaugeBar.xml");
		child.second->SetName(child.first.c_str());

		texture->SetCurrentAnimation(child.first);
		texture->SetAnimationMode(AnimationMode::Play);
	}
}

void GaugeBarComponent::Update()
{
	// 현재 퍼센트를 구함
	float hpPersent = GetHp() / GetMaxHp();
	float manaPersent = GetMana() / GetMaxMana();

	if (hp <= 0)
	{
		const auto& parent = actor->GetComponent<TransformComponent>()->GetParent().lock()->GetActor();
		bool animatorExist = parent->HasCompoent<AnimatorComponent>();

		// 죽은 객체가 플레이어인 경우
		if (parent->GetName() == "Player")
		{
			assert(false && "Game Over");
			exit(0);
		}

		// 죽은 객체가 몬스터인 경우
		else if (!isDead && animatorExist)
		{
			context->GetSubSystem<ScoreUI>()->AddScore(1);

			std::shared_ptr animator = parent->GetComponent<AnimatorComponent>();

			if (parent->GetName().substr(1) == "GreenSlimeMonster")
				parent->GetComponent<TransformComponent>()->SetScale(D3DXVECTOR3(65.0f, 50.0f, 1.0f));

			else
				parent->GetComponent<TransformComponent>()->SetScale(D3DXVECTOR3(70.0f, 100.0f, 1.0f));
			
			// 죽은 애니메이션 실행
			if (animator->GetNowKeyFrameName() == "LeftAttack" || animator->GetNowKeyFrameName() == "LeftMove")
				parent->GetComponent<AnimatorComponent>()->SetCurrentAnimation("LeftDeath");

			else
				parent->GetComponent<AnimatorComponent>()->SetCurrentAnimation("RightDeath");

			isDead = true;
		}

		// 죽는 애니메이션이 없거나 애니메이션이 끝난 경우 사망 처리
		if (!animatorExist || parent->GetComponent<AnimatorComponent>()->GetAnimationMode() == AnimationMode::Pause)
		{
			Destroy();
			return;
		}

		hpPersent = 0.0f;
		manaPersent = 0.0f;
	}

	// 게이지의 크기 설정
	children.at("HpGauge")->GetComponent<TransformComponent>()->SetLocalScale(D3DXVECTOR3(gaugeScalePos.hpBarGaugeSize.x * hpPersent, gaugeScalePos.hpBarGaugeSize.y, 1.0f));
	children.at("ManaGauge")->GetComponent<TransformComponent>()->SetLocalScale(D3DXVECTOR3(gaugeScalePos.manaBarGaugeSize.x * manaPersent, gaugeScalePos.manaBarGaugeSize.y, 1.0f));

	// 게이지의 위치 설정
	children.at("HpGauge")->GetComponent<TransformComponent>()->SetLocalPosition(
		D3DXVECTOR3(gaugeScalePos.hpBarGaugePos.x - gaugeScalePos.hpBarGaugeSize.x * (1 - hpPersent) / 2, gaugeScalePos.hpBarGaugePos.y, 0.0f));
	children.at("ManaGauge")->GetComponent<TransformComponent>()->SetLocalPosition(
		D3DXVECTOR3(gaugeScalePos.manaBarGaugePos.x - gaugeScalePos.manaBarGaugeSize.x * (1 - manaPersent) / 2, gaugeScalePos.manaBarGaugePos.y, 0.0f));

	// 게이지 바 위치 조정
	const auto& parentPos = actor->GetComponent<TransformComponent>()->GetParent().lock()->GetPosition();

	actor->GetComponent<TransformComponent>()->SetPosition(parentPos);
	actor->GetComponent<TransformComponent>()->SetScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
}

void GaugeBarComponent::Destroy()
{
	auto parent = actor->GetComponent<TransformComponent>()->GetParent().lock()->GetActor();

	// 게이지바 구성 요소 제거
	for (const auto& child : children)
		actor->GetScene()->EreaseActor(child.second);

	// 적 총알 제거
	if (parent->HasCompoent<BulletComponent>())
	{
		for (const auto& bullet : parent->GetComponent<BulletComponent>()->GetBullets())
			actor->GetScene()->EreaseActor(bullet);
	}

	// 게이지 바 제거
	parent->Destroy();
	parent->~Actor();
	actor->SetName("");
}

void GaugeBarComponent::SetBarTransform(const BarTransform& barTransform, const float hpPersent, const float manaPersent)
{
	this->stateBarTransform = barTransform; 	
	
	// 자식 객체 기준 부모 클래스(게이지 바 객체)에서의 위치와 크기를 지정함
	children.at("HpBar")->GetComponent<TransformComponent>()->SetLocalPosition(D3DXVECTOR3(stateBarTransform.hpBarPos.x, stateBarTransform.hpBarPos.y, 0.0f));
	children.at("HpBar")->GetComponent<TransformComponent>()->SetLocalScale(D3DXVECTOR3(stateBarTransform.hpBarSize.x, stateBarTransform.hpBarSize.y, 1.0f));

	children.at("ManaBar")->GetComponent<TransformComponent>()->SetLocalPosition(D3DXVECTOR3(stateBarTransform.manaBarPos.x, stateBarTransform.manaBarPos.y, 0.0f));
	children.at("ManaBar")->GetComponent<TransformComponent>()->SetLocalScale(D3DXVECTOR3(stateBarTransform.manaBarSize.x, stateBarTransform.manaBarSize.y, 1.0f));

	// 게이지 바를 입력 받은 위치에서 적절하게 위치하도록 설정
	gaugeScalePos.hpBarGaugePos = D3DXVECTOR2(stateBarTransform.hpBarPos.x + stateBarTransform.hpBarSize.x * 0.08f, stateBarTransform.hpBarPos.y - 1.0f);
	gaugeScalePos.manaBarGaugePos = D3DXVECTOR2(stateBarTransform.manaBarPos.x + stateBarTransform.manaBarSize.x * 0.08f, stateBarTransform.manaBarPos.y - 1.0f);

	gaugeScalePos.hpBarGaugeSize = D3DXVECTOR2(stateBarTransform.hpBarSize.x * 0.8f, stateBarTransform.hpBarSize.y / 2.0f);
	gaugeScalePos.manaBarGaugeSize = D3DXVECTOR2(stateBarTransform.manaBarSize.x * 0.8f, stateBarTransform.manaBarSize.y / 2.0f);

	children.at("HpGauge")->GetComponent<TransformComponent>()->SetLocalPosition(D3DXVECTOR3(gaugeScalePos.hpBarGaugePos.x, gaugeScalePos.hpBarGaugePos.y, 0.0f));
	children.at("HpGauge")->GetComponent<TransformComponent>()->SetLocalScale(D3DXVECTOR3(gaugeScalePos.hpBarGaugeSize.x * hpPersent, gaugeScalePos.hpBarGaugeSize.y, 1.0f));

	children.at("ManaGauge")->GetComponent<TransformComponent>()->SetLocalPosition(D3DXVECTOR3(gaugeScalePos.manaBarGaugePos.x, gaugeScalePos.manaBarGaugePos.y, 0.0f));
	children.at("ManaGauge")->GetComponent<TransformComponent>()->SetLocalScale(D3DXVECTOR3(gaugeScalePos.manaBarGaugeSize.x * manaPersent, gaugeScalePos.manaBarGaugeSize.y, 1.0f));
}
