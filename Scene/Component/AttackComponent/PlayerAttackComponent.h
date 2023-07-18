#pragma once
#include "../IComponent.h"

#include "../../Actor.h"
#include "Scene/Component/MoveComponent/TransformComponent.h"

// 장거리인지 단거리인지 판별
enum class PlayerBulletStyle { None, Long, Short };

class PlayerAttackComponent : public IComponent
{
public:
	using IComponent::IComponent;
	~PlayerAttackComponent() = default;

	// IComponent을(를) 통해 상속됨
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
	std::map<std::string, std::shared_ptr<Actor>> sword;		// 방향에 따른 검 스프라이트 객체
	std::map<std::string, std::shared_ptr<Actor>> shot;			// 방향에 따른 피격 스프라이트 객체
	std::list<std::shared_ptr<Actor>> bullet;					// 총알 스프라이트 객체
	std::shared_ptr<Actor> gun;									// 총 스프라이트 객체

	float delay = 0.0f;											// 사격 딜레이 (단위 : 초)
	float nowDelay = 0.0f;										// 남은 딜레이
	float animationPlay = 0.0f;									// 애니메이션 출력이 남은 시간

	PlayerBulletStyle bulletStyle = PlayerBulletStyle::None;	// 공격 장, 단거리 구분
	float bulletSpeed = 0.0f;									// 총알 속도 (장거리 공격인 경우)
	float attackRange = 0.0f;									// 객체의 사거리
	float damage = 0.0f;										// 객체의 공격력

	bool isAttack = false;										// 현재 공격중인지 확인
	bool isDamage = true;										// 다른 몬스터에게 피해를 줬는지 확인

	Timer* timer;												// 타임머
};

