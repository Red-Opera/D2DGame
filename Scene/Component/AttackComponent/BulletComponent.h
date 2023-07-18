#pragma once
#include "../IComponent.h"

/*
	Component ����
		1. TraceComponent�� ���� �߰��Ѵ�.
		2. TraceComponent�� Ÿ���� �����Ѵ�.
		3. ���� ���¿� �����̵� �ӵ��� �����Ѵ�.
		4. ������ ��������Ʈ �̸��� ��������Ʈ ��θ� �Է���
*/

// ��Ÿ����� �ܰŸ����� �Ǻ�
enum class BulletStyle { None, Long, Short };

class BulletComponent : public IComponent
{
public:
	using IComponent::IComponent;
	
	// IComponent��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Destroy() override;
	
	void SetStyleSpeed(BulletStyle style, float speed) { this->bulletStyle = style; this->speed = speed; }
	void SetSprite(std::string spriteName, std::string spritePath) { this->spriteFrameName = spriteName, this->spritePath = spritePath; }

	const std::list<std::shared_ptr<class Actor>> GetBullets() const { return bullets; }

private:
	void PossibleProceed();

private:
	std::string spriteFrameName;						// �Ѿ� ��������Ʈ xml�̸�
	std::string spritePath;								// �Ѿ� ��������Ʈ xml���� ���

	std::list<std::shared_ptr<class Actor>> bullets;	// �ش� ��ü���� ������ �Ѿ�

	BulletStyle bulletStyle = BulletStyle::None;		// ��Ÿ� ����
	Timer *timer = 0;									// Ÿ�Ӹ�
	float nowDelay = 0;									// ��ݱ��� ���� �ð�
	float speed = 0;									// �Ѿ� �ӵ�

};

