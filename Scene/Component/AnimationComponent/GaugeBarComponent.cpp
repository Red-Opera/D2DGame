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
	// actor�� ������ �ٸ� �ǹ�
	if (!actor->HasCompoent<TransformComponent>())
	{
		assert(false && "ERROR (GaugetBarComponent2) : ������ �ٿ� TransformComponent�� ����");
		return;
	}

	// ü�� ���� �ڽ� Ŭ���� ����
	children.emplace(std::string("HpBar"), actor->GetScene()->CreateActor());
	children.emplace(std::string("ManaBar"), actor->GetScene()->CreateActor());
	children.emplace(std::string("HpGauge"), actor->GetScene()->CreateActor());
	children.emplace(std::string("ManaGauge"), actor->GetScene()->CreateActor());

	for (const auto& child : children)
	{
		child.second->AddComponent<MeshRendererComponent>();
		child.second->AddComponent<TransformComponent>();

		// ü�� ���� �ڽ� Ŭ���� ü�� �� ������ ��ü�� �θ�� ���� �� ������ ����
		child.second->GetComponent<TransformComponent>()->SetParent(actor->GetComponent<TransformComponent>());
		actor->GetComponent<TransformComponent>()->AddChild(child.second->GetComponent<TransformComponent>());

		// ������ ��ü�� �ؽ��� ����
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
	// ���� �ۼ�Ʈ�� ����
	float hpPersent = GetHp() / GetMaxHp();
	float manaPersent = GetMana() / GetMaxMana();

	if (hp <= 0)
	{
		const auto& parent = actor->GetComponent<TransformComponent>()->GetParent().lock()->GetActor();
		bool animatorExist = parent->HasCompoent<AnimatorComponent>();

		// ���� ��ü�� �÷��̾��� ���
		if (parent->GetName() == "Player")
		{
			assert(false && "Game Over");
			exit(0);
		}

		// ���� ��ü�� ������ ���
		else if (!isDead && animatorExist)
		{
			context->GetSubSystem<ScoreUI>()->AddScore(1);

			std::shared_ptr animator = parent->GetComponent<AnimatorComponent>();

			if (parent->GetName().substr(1) == "GreenSlimeMonster")
				parent->GetComponent<TransformComponent>()->SetScale(D3DXVECTOR3(65.0f, 50.0f, 1.0f));

			else
				parent->GetComponent<TransformComponent>()->SetScale(D3DXVECTOR3(70.0f, 100.0f, 1.0f));
			
			// ���� �ִϸ��̼� ����
			if (animator->GetNowKeyFrameName() == "LeftAttack" || animator->GetNowKeyFrameName() == "LeftMove")
				parent->GetComponent<AnimatorComponent>()->SetCurrentAnimation("LeftDeath");

			else
				parent->GetComponent<AnimatorComponent>()->SetCurrentAnimation("RightDeath");

			isDead = true;
		}

		// �״� �ִϸ��̼��� ���ų� �ִϸ��̼��� ���� ��� ��� ó��
		if (!animatorExist || parent->GetComponent<AnimatorComponent>()->GetAnimationMode() == AnimationMode::Pause)
		{
			Destroy();
			return;
		}

		hpPersent = 0.0f;
		manaPersent = 0.0f;
	}

	// �������� ũ�� ����
	children.at("HpGauge")->GetComponent<TransformComponent>()->SetLocalScale(D3DXVECTOR3(gaugeScalePos.hpBarGaugeSize.x * hpPersent, gaugeScalePos.hpBarGaugeSize.y, 1.0f));
	children.at("ManaGauge")->GetComponent<TransformComponent>()->SetLocalScale(D3DXVECTOR3(gaugeScalePos.manaBarGaugeSize.x * manaPersent, gaugeScalePos.manaBarGaugeSize.y, 1.0f));

	// �������� ��ġ ����
	children.at("HpGauge")->GetComponent<TransformComponent>()->SetLocalPosition(
		D3DXVECTOR3(gaugeScalePos.hpBarGaugePos.x - gaugeScalePos.hpBarGaugeSize.x * (1 - hpPersent) / 2, gaugeScalePos.hpBarGaugePos.y, 0.0f));
	children.at("ManaGauge")->GetComponent<TransformComponent>()->SetLocalPosition(
		D3DXVECTOR3(gaugeScalePos.manaBarGaugePos.x - gaugeScalePos.manaBarGaugeSize.x * (1 - manaPersent) / 2, gaugeScalePos.manaBarGaugePos.y, 0.0f));

	// ������ �� ��ġ ����
	const auto& parentPos = actor->GetComponent<TransformComponent>()->GetParent().lock()->GetPosition();

	actor->GetComponent<TransformComponent>()->SetPosition(parentPos);
	actor->GetComponent<TransformComponent>()->SetScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
}

void GaugeBarComponent::Destroy()
{
	auto parent = actor->GetComponent<TransformComponent>()->GetParent().lock()->GetActor();

	// �������� ���� ��� ����
	for (const auto& child : children)
		actor->GetScene()->EreaseActor(child.second);

	// �� �Ѿ� ����
	if (parent->HasCompoent<BulletComponent>())
	{
		for (const auto& bullet : parent->GetComponent<BulletComponent>()->GetBullets())
			actor->GetScene()->EreaseActor(bullet);
	}

	// ������ �� ����
	parent->Destroy();
	parent->~Actor();
	actor->SetName("");
}

void GaugeBarComponent::SetBarTransform(const BarTransform& barTransform, const float hpPersent, const float manaPersent)
{
	this->stateBarTransform = barTransform; 	
	
	// �ڽ� ��ü ���� �θ� Ŭ����(������ �� ��ü)������ ��ġ�� ũ�⸦ ������
	children.at("HpBar")->GetComponent<TransformComponent>()->SetLocalPosition(D3DXVECTOR3(stateBarTransform.hpBarPos.x, stateBarTransform.hpBarPos.y, 0.0f));
	children.at("HpBar")->GetComponent<TransformComponent>()->SetLocalScale(D3DXVECTOR3(stateBarTransform.hpBarSize.x, stateBarTransform.hpBarSize.y, 1.0f));

	children.at("ManaBar")->GetComponent<TransformComponent>()->SetLocalPosition(D3DXVECTOR3(stateBarTransform.manaBarPos.x, stateBarTransform.manaBarPos.y, 0.0f));
	children.at("ManaBar")->GetComponent<TransformComponent>()->SetLocalScale(D3DXVECTOR3(stateBarTransform.manaBarSize.x, stateBarTransform.manaBarSize.y, 1.0f));

	// ������ �ٸ� �Է� ���� ��ġ���� �����ϰ� ��ġ�ϵ��� ����
	gaugeScalePos.hpBarGaugePos = D3DXVECTOR2(stateBarTransform.hpBarPos.x + stateBarTransform.hpBarSize.x * 0.08f, stateBarTransform.hpBarPos.y - 1.0f);
	gaugeScalePos.manaBarGaugePos = D3DXVECTOR2(stateBarTransform.manaBarPos.x + stateBarTransform.manaBarSize.x * 0.08f, stateBarTransform.manaBarPos.y - 1.0f);

	gaugeScalePos.hpBarGaugeSize = D3DXVECTOR2(stateBarTransform.hpBarSize.x * 0.8f, stateBarTransform.hpBarSize.y / 2.0f);
	gaugeScalePos.manaBarGaugeSize = D3DXVECTOR2(stateBarTransform.manaBarSize.x * 0.8f, stateBarTransform.manaBarSize.y / 2.0f);

	children.at("HpGauge")->GetComponent<TransformComponent>()->SetLocalPosition(D3DXVECTOR3(gaugeScalePos.hpBarGaugePos.x, gaugeScalePos.hpBarGaugePos.y, 0.0f));
	children.at("HpGauge")->GetComponent<TransformComponent>()->SetLocalScale(D3DXVECTOR3(gaugeScalePos.hpBarGaugeSize.x * hpPersent, gaugeScalePos.hpBarGaugeSize.y, 1.0f));

	children.at("ManaGauge")->GetComponent<TransformComponent>()->SetLocalPosition(D3DXVECTOR3(gaugeScalePos.manaBarGaugePos.x, gaugeScalePos.manaBarGaugePos.y, 0.0f));
	children.at("ManaGauge")->GetComponent<TransformComponent>()->SetLocalScale(D3DXVECTOR3(gaugeScalePos.manaBarGaugeSize.x * manaPersent, gaugeScalePos.manaBarGaugeSize.y, 1.0f));
}
