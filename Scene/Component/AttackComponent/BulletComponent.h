#pragma once
#include "../IComponent.h"

/*
	Component 사용법
		1. TraceComponent를 먼저 추가한다.
		2. TraceComponent의 타겟을 설정한다.
		3. 공격 형태와 공격이동 속도를 지정한다.
		4. 가져올 스프라이트 이름과 스프라이트 경로를 입력함
*/

// 장거리인지 단거리인지 판별
enum class BulletStyle { None, Long, Short };

class BulletComponent : public IComponent
{
public:
	using IComponent::IComponent;
	
	// IComponent을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Destroy() override;
	
	void SetStyleSpeed(BulletStyle style, float speed) { this->bulletStyle = style; this->speed = speed; }
	void SetSprite(std::string spriteName, std::string spritePath) { this->spriteFrameName = spriteName, this->spritePath = spritePath; }

	const std::list<std::shared_ptr<class Actor>> GetBullets() const { return bullets; }

private:
	void PossibleProceed();

private:
	std::string spriteFrameName;						// 총알 스프라이트 xml이름
	std::string spritePath;								// 총알 스프라이트 xml파일 경로

	std::list<std::shared_ptr<class Actor>> bullets;	// 해당 객체에서 생성된 총알

	BulletStyle bulletStyle = BulletStyle::None;		// 사거리 구분
	Timer *timer = 0;									// 타임머
	float nowDelay = 0;									// 사격까지 남은 시간
	float speed = 0;									// 총알 속도

};

