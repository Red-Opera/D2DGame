#include "stdafx.h"

#include "ChangeAttackTypeManager.h"

#include "Scene/Actor.h"
#include "Scene/Component/AttackComponent/PlayerAttackComponent.h"

#define HOLDPRESSKEYBOARD(input, key) (input->GetKeyState(key) == KEY_HOLD || input->GetKeyState(key) == KEY_DOWN)
#define ONLYPRESSKEYBOARD(input, key) (input->GetKeyState(key) == KEY_DOWN)

// ���漱��
std::shared_ptr<Actor> ChangeAttackTypeManager::target = nullptr;
std::queue<int> ChangeAttackTypeManager::key;
std::queue<PlayerBulletStyle> ChangeAttackTypeManager::style;
std::queue<float> ChangeAttackTypeManager::bulletSpeed;

bool ChangeAttackTypeManager::Initialize()
{
	input = context->GetSubSystem<InputManager>();
	return false;
}

void ChangeAttackTypeManager::Update()
{
	// ���� �Է��� ���� �����ϱ� ���� �� ������
	auto copyKey = ChangeAttackTypeManager::key;
	auto copyStyle = ChangeAttackTypeManager::style;
	auto copyBulletSpeed = ChangeAttackTypeManager::bulletSpeed;
	
	while (!copyKey.empty())
	{
		if (HOLDPRESSKEYBOARD(input, copyKey.front()))
		{
			// �̹� ������ ����� ���⸦ �������� ����
			if (copyStyle.front() == ChangeAttackTypeManager::target->GetComponent<PlayerAttackComponent>()->GetBulletStyle()) return;

			target->GetComponent<PlayerAttackComponent>()->SetStyleSpeed(copyStyle.front(), copyBulletSpeed.front());
		}
	
		copyKey.pop();
		if (copyStyle.front() == PlayerBulletStyle::Long) copyBulletSpeed.pop();
		copyStyle.pop();
	}
}

void ChangeAttackTypeManager::ChangeType(std::shared_ptr<Actor> target1, std::queue<int> key1, std::queue<PlayerBulletStyle> style1, std::queue<float> bulletSpeed1)
{
	if (!target1->HasCompoent<PlayerAttackComponent>())
	{
		assert(false && "Error ChangeAttackTypeManager1 : �ش� ��ü�� PlayAttackComponent�� ����");
		return;
	}
	
	ChangeAttackTypeManager::target = target1;
	
	ChangeAttackTypeManager::key = key1;
	ChangeAttackTypeManager::style = style1;
	ChangeAttackTypeManager::bulletSpeed = bulletSpeed1;
}