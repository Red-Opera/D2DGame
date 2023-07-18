// Program.cpp
// 프로그램 진행하기 위한 파일

#include "stdafx.h"
#include "Core/Window.h"
#include "Core/Engine.h"

int APIENTRY WinMain
(
	HINSTANCE hInstance,
	HINSTANCE prevInstance,
	LPSTR lpszCmdParam,
	int nCmdShow
)
{
  //std::cout << GUID_Generator::Generate() << std::endl;

	// 윈도우 생성
	Window::Create(hInstance, 1216, 939);
	Window::Show();

	Settings::Get().SetWindowHandle(Window::global_handle);					// 현재 실행중인 Window창 핸들을 Setting에 있는 Window핸들에 저장
	Settings::Get().SetWindowInstance(Window::global_instance);
	Settings::Get().SetWidth(static_cast<float>(Window::GetWidth()));		// Setting클래스의 Width에 화면 너비를 저장
	Settings::Get().SetHeigh(static_cast<float>(Window::GetHeight()));		// Setting클래스의 Heigh에 화면 높이를 저장

	// Window창을 랜더링하거나 업데이트하는 기능을 가져오는 변수
	std::unique_ptr<Engine> engine = std::make_unique<Engine>();

	while (Window::Update())
	{
		engine->Update();
		engine->Render();
	}

	// 윈도우 파괴
	Window::Destroy();
}