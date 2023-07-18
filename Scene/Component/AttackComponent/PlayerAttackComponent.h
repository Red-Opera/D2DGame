#pragma once
#include "../IComponent.h"

#include "../../Actor.h"
#include "Scene/Component/MoveComponent/TransformComponent.h"

// ��Ÿ����� �ܰŸ����� �Ǻ�
enum class PlayerBulletStyle { None, Long, Short };

class PlayerAttackComponent : public IComponent
{
public:
	using IComponent::IComponent;
	~PlayerAttackComponent() = default;

	// IComponent��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Destroy() override;

	void SetAttackDelay(const float delay) { this->delay = delay; }
	float GetAttackDelay() { return delay; }

	void SetAttackRange(const float attackRange) { this->attackRange = attackRange; }
	const float GetAttackRange() const { return attackRange; }

	void SetDamage(const float damage) { this->damage = damage; }
	const float GetDamage() const { return damage; }

	void SetStyleSpeed(PlayerBulletStyle style, float bulletSpeed = 0.0f) { this->bulletStyle = style; this->bulletSpeed = bulletSpeed; }
	const PlayerBulletStyle GetBulletStyle() const { return bulletStyle; }

	const void AddBullet(const std::shared_ptr<Actor> bullet) { this->bullet.emplace_back(bullet); }
	const std::list<std::shared_ptr<Actor>>& GetBullets() const { return bullet; }

	const std::shared_ptr<TransformComponent> GetGunTransform() const { return gun->GetComponent<TransformComponent>(); }

private:
	static bool FindRightAttackSprite(std::string& spriteName) { return (spriteName.substr(spriteName.size() - 11, 11) == "RightAttack"); }
	static bool FindLeftAttackSprite(std::string& spriteName) { return (spriteName.substr(spriteName.size() - 10, 10) == "LeftAttack"); }

private:
	std::map<std::string, std::shared_ptr<Actor>> sword;		// ���⿡ ���� �� ��������Ʈ ��ü
	std::map<std::string, std::shared_ptr<Actor>> shot;			// ���⿡ ���� �ǰ� ��������Ʈ ��ü
	std::list<std::shared_ptr<Actor>> bullet;					// �Ѿ� ��������Ʈ ��ü
	std::shared_ptr<Actor> gun;									// �� ��������Ʈ ��ü

	float delay = 0.0f;											// ��� ������ (���� : ��)
	float nowDelay = 0.0f;										// ���� ������
	float animationPlay = 0.0f;									// �ִϸ��̼� ����� ���� �ð�

	PlayerBulletStyle bulletStyle = PlayerBulletStyle::None;	// ���� ��, �ܰŸ� ����
	float bulletSpeed = 0.0f;									// �Ѿ� �ӵ� (��Ÿ� ������ ���)
	float attackRange = 0.0f;									// ��ü�� ��Ÿ�
	float damage = 0.0f;										// ��ü�� ���ݷ�

	bool isAttack = false;										// ���� ���������� Ȯ��
	bool isDamage = true;										// �ٸ� ���Ϳ��� ���ظ� ����� Ȯ��

	Timer* timer;												// Ÿ�Ӹ�
};

