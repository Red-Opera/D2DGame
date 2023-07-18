#pragma once
#include "ISubSystem.h"

// 키보드의 입력 상태를 표시하는 Enum
enum KEY_STATE
{
	KEY_FREE,
	KEY_DOWN,
	KEY_UP,
	KEY_HOLD
};

// 마우스의 입력 상태를 표시하는 Enum
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

	// ISubSystem을(를) 통해 상속됨
	virtual bool Initialize() override;
	virtual void Update() override;

	const POINT& GetMousePosition() const { return mouse_position; }

	const KEY_STATE& GetKeyState(BYTE key) const { return key_state[key]; }
	const KEY_STATE& GetMouseState(MOUSE_BUTTON button) const { return mouse_state[button]; }

private:
	LPDIRECTINPUT8 direct_input_device = nullptr;		// direct input관련 자원 입력
	LPDIRECTINPUTDEVICE8 key_device = nullptr;			// 키보드 장치를 의미하는 변수
	LPDIRECTINPUTDEVICE8 mouse_device = nullptr;		// 마우스 장치를 의미하는 변수

	// 각각의 키 상태를 의미하는 변수
	BYTE input_key_state[256];
	DIMOUSESTATE input_mouse_state;

	// 키 상태를 저장하고 값들을 가져갈 때 사용
	KEY_STATE key_state[256];
	KEY_STATE mouse_state[256];
	POINT mouse_position;
};