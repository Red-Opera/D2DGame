#pragma once
#include "ISubSystem.h"

// Ű������ �Է� ���¸� ǥ���ϴ� Enum
enum KEY_STATE
{
	KEY_FREE,
	KEY_DOWN,
	KEY_UP,
	KEY_HOLD
};

// ���콺�� �Է� ���¸� ǥ���ϴ� Enum
enum MOUSE_BUTTON
{
	MOUSE_LEFT,
	MOUSE_RIGHT,
	MOUSE_MIDDLE
};

class InputManager final : public ISubSystem
{
public:
	using ISubSystem::ISubSystem;
	~InputManager();

	// ISubSystem��(��) ���� ��ӵ�
	virtual bool Initialize() override;
	virtual void Update() override;

	const POINT& GetMousePosition() const { return mouse_position; }

	const KEY_STATE& GetKeyState(BYTE key) const { return key_state[key]; }
	const KEY_STATE& GetMouseState(MOUSE_BUTTON button) const { return mouse_state[button]; }

private:
	LPDIRECTINPUT8 direct_input_device = nullptr;		// direct input���� �ڿ� �Է�
	LPDIRECTINPUTDEVICE8 key_device = nullptr;			// Ű���� ��ġ�� �ǹ��ϴ� ����
	LPDIRECTINPUTDEVICE8 mouse_device = nullptr;		// ���콺 ��ġ�� �ǹ��ϴ� ����

	// ������ Ű ���¸� �ǹ��ϴ� ����
	BYTE input_key_state[256];
	DIMOUSESTATE input_mouse_state;

	// Ű ���¸� �����ϰ� ������ ������ �� ���
	KEY_STATE key_state[256];
	KEY_STATE mouse_state[256];
	POINT mouse_position;
};