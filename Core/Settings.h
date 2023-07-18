#pragma once
// Settings.h
// Window�� ������ �ڿ��� �����ϴ� ���

#include "stdafx.h"

// �ν��Ͻ��� ������ �ʿ䰡 ���� ������ �̱������� ����� ��
class Settings final
{
public:
	static Settings& Get()
	{
		static Settings instance;
		return instance;
	}

	// handle, width, height�� Getter, Setter ����
	HWND GetWindowHandle() { return handle; }
	void SetWindowHandle(HWND handle) { this->handle = handle; }	// ���α׷��� ������ �� �ڵ��� SettingŬ������ ������ �� ���

	const float& GetWidth() { return width; }
	void SetWidth(const float& width) { this->width = width; }		// ���α׷��� ������ �� ȭ���� �ʺ� ����

	const float& GetHeight() { return height; }
	void SetHeigh(const float& height) { this->height = height; }	// ���α׷��� ������ �� ȭ���� ���̸� ����

	// HINSTANCE Getter, Setter ����
	HINSTANCE GetWindowInstance() { return instance; }
	void SetWindowInstance(HINSTANCE instance) { this->instance = instance; }

private:
	Settings() = default;
	~Settings() = default;

	// ���縦 �� �� ������ ������Ի����ڿ� ��������ڸ� ������
	Settings(const Settings&) = delete;			
	Settings& operator=(const Settings&) = delete;

private:
	HWND handle = nullptr;
	HINSTANCE instance = nullptr;

	float width = 0.0f;
	float height = 0.0f;
};