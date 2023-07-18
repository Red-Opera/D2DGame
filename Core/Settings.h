#pragma once
// Settings.h
// Window가 가지는 자원을 관리하는 헤더

#include "stdafx.h"

// 인스턴스가 여러일 필요가 없기 때문에 싱글톤으로 만들어 줌
class Settings final
{
public:
	static Settings& Get()
	{
		static Settings instance;
		return instance;
	}

	// handle, width, height의 Getter, Setter 세팅
	HWND GetWindowHandle() { return handle; }
	void SetWindowHandle(HWND handle) { this->handle = handle; }	// 프로그램이 시작할 때 핸들을 Setting클래스에 저장할 때 사용

	const float& GetWidth() { return width; }
	void SetWidth(const float& width) { this->width = width; }		// 프로그램이 시작할 때 화면의 너비를 받음

	const float& GetHeight() { return height; }
	void SetHeigh(const float& height) { this->height = height; }	// 프로그램이 시작할 때 화면의 높이를 받음

	// HINSTANCE Getter, Setter 세팅
	HINSTANCE GetWindowInstance() { return instance; }
	void SetWindowInstance(HINSTANCE instance) { this->instance = instance; }

private:
	Settings() = default;
	~Settings() = default;

	// 복사를 할 수 없도록 복사대입생성자와 복사생성자를 제거함
	Settings(const Settings&) = delete;			
	Settings& operator=(const Settings&) = delete;

private:
	HWND handle = nullptr;
	HINSTANCE instance = nullptr;

	float width = 0.0f;
	float height = 0.0f;
};