#include "stdafx.h"
#include "InputManager.h"

InputManager::~InputManager()
{
	if (mouse_device != nullptr)
	{
		mouse_device->Unacquire();
		SAFE_RELEASE(mouse_device);
	}

	if (key_device != nullptr)
	{
		key_device->Unacquire();
		SAFE_RELEASE(key_device);
	}

	SAFE_RELEASE(direct_input_device);
}

bool InputManager::Initialize()
{
	// ========================================
	// Initialize
	// ========================================

	// 변수 데이터 초기화
	ZeroMemory(input_key_state, sizeof(BYTE) * 256);
	ZeroMemory(&input_mouse_state, sizeof(DIMOUSESTATE));

	ZeroMemory(key_state, sizeof(KEY_STATE) * 256);
	ZeroMemory(mouse_state, sizeof(KEY_STATE) * 3);
	ZeroMemory(&mouse_position, sizeof(POINT));

	// ========================================
	// Create Direct Input Device
	// ========================================

	HRESULT hr = DirectInput8Create
	(
		Settings::Get().GetWindowInstance(),				// Window 핸들
		DIRECTINPUT_VERSION, 								// Direct Input 버전
		IID_IDirectInput8, 									// GUID ID값
		reinterpret_cast<void**>(&direct_input_device), 	// 완성된 값을 받을 변수
		nullptr												// 실패했을 경우 받을 변수
	);

	if (FAILED(hr)) { assert(false); return false; }

	// ========================================
	// Create Key Device
	// ========================================

	// 키보드 장치를 제작
	hr = direct_input_device->CreateDevice
	(
		GUID_SysKeyboard,	// 어떤 장치인지 판단하는 GUID
		&key_device,		// 키보드 장치를 받을 변수
		nullptr
	);
	if (FAILED(hr)) { assert(false); return false; }

	// 데이터를 받을 때 keyBoard의 데이터 형식으로 설정함
	key_device->SetDataFormat(&c_dfDIKeyboard);

	// 어느 수준까지 상호작용할 것이지 설정
	hr = key_device->SetCooperativeLevel
	(
		Settings::Get().GetWindowHandle(),		// Window 핸들
		DISCL_NONEXCLUSIVE | DISCL_FOREGROUND	// 설정 : 프로그램이 실행하는 동안 다른 프로그램이 상호작용할 수 있게 설정, 전면에 있을 때만 상호작용
	);
	if (FAILED(hr)) { assert(false); return false; }

	// 키보드 장치의 접근권한을 가져올 때까지 무한히 접근권한을 불려옴
	while (key_device->Acquire() == DIERR_INPUTLOST);

	// ========================================
	// Create Mouse Device
	// ========================================

	// 마우스 장치를 제작
	hr = direct_input_device->CreateDevice
	(
		GUID_SysMouse,		// 어떤 장치인지 판단하는 GUID
		&mouse_device,		// 마우스 장치를 받을 변수
		nullptr
	);
	if (FAILED(hr)) { assert(false); return false; }

	// 데이터를 받을 때 Mouse의 데이터 형식으로 설정함
	mouse_device->SetDataFormat(&c_dfDIMouse);

	// 어느 수준까지 상호작용할 것이지 설정
	hr = mouse_device->SetCooperativeLevel
	(
		Settings::Get().GetWindowHandle(),		// Window 핸들
		DISCL_NONEXCLUSIVE | DISCL_FOREGROUND	// 설정 : 프로그램이 실행하는 동안 다른 프로그램이 상호작용할 수 있게 설정, 전면에 있을 때만 상호작용
	);
	if (FAILED(hr)) { assert(false); return false; }

	// 마우스 장치의 접근권한을 가져올 때까지 무한히 접근권한을 불려옴
	while (mouse_device->Acquire() == DIERR_INPUTLOST);

	return true;
}

void InputManager::Update()
{
	if (key_device == nullptr || mouse_device == nullptr)
	{
		assert(false);
		return;
	}

	// ========================================
	// Read Data
	// ========================================

	HRESULT hr = key_device->GetDeviceState(256, input_key_state);	// 키보드의 상태값을 받아옴
	if (FAILED(hr))		// 창이 전면이 아닐 경우 키보트의 상태값을 받지 못할 수 있음
	{
		while (key_device->Acquire() == DIERR_INPUTLOST);
		hr = key_device->GetDeviceState(256, &input_key_state);
		if (FAILED(hr)) return;
	}

	hr = mouse_device->GetDeviceState(sizeof(DIMOUSESTATE), &input_mouse_state);
	if (FAILED(hr))
	{
		while (mouse_device->Acquire() == DIERR_INPUTLOST);
		hr = mouse_device->GetDeviceState(sizeof(DIMOUSESTATE), &input_mouse_state);
		if (FAILED(hr)) return;
	}

	// ========================================
	// Update Mouse
	// ========================================

	GetCursorPos(&mouse_position);
	ScreenToClient(Settings::Get().GetWindowHandle(), &mouse_position);

	mouse_position.x -= static_cast<LONG>(Settings::Get().GetWidth() / 2);
	mouse_position.y = -mouse_position.y + static_cast<LONG>(Settings::Get().GetHeight() / 2);

  //system("cls");
  //std::cout << "(" << mouse_position.x << ", " << mouse_position.y << ")" << std::endl;
  //Sleep(25);

	for (int button = 0; button < 3; button++)
	{
		if (input_mouse_state.rgbButtons[button] & 0x80)
		{
			switch (mouse_state[button])
			{
			case KEY_FREE:
			case KEY_UP:
				mouse_state[button] = KEY_DOWN; break;	

			case KEY_DOWN:
				mouse_state[button] = KEY_HOLD; break;

			default:
				break;
			}
		}

		else
		{
			switch (mouse_state[button])
			{
			case KEY_HOLD:
			case KEY_DOWN:
				mouse_state[button] = KEY_UP; break;

			case KEY_UP:
				mouse_state[button] = KEY_FREE; break;

			default:
				break;
			}
		}
	}

	// ========================================
	// Update Key
	// ========================================

	for (int key = 0; key < 256; key++)
	{
		if (input_key_state[key] & 0x80)
		{
			switch (key_state[key])
			{
			case KEY_FREE:
			case KEY_UP:
				key_state[key] = KEY_DOWN; break;

			case KEY_DOWN:
				key_state[key] = KEY_HOLD; break;

			default:
				break;
			}
		}

		else
		{
			switch (key_state[key])
			{
			case KEY_HOLD:
			case KEY_DOWN:
				key_state[key] = KEY_UP; break;

			case KEY_UP:
				key_state[key] = KEY_FREE; break;

			default:
				break;
			}
		}
	}
}