// Program.cpp
// ���α׷� �����ϱ� ���� ����

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

	// ������ ����
	Window::Create(hInstance, 1216, 939);
	Window::Show();

	Settings::Get().SetWindowHandle(Window::global_handle);					// ���� �������� Windowâ �ڵ��� Setting�� �ִ� Window�ڵ鿡 ����
	Settings::Get().SetWindowInstance(Window::global_instance);
	Settings::Get().SetWidth(static_cast<float>(Window::GetWidth()));		// SettingŬ������ Width�� ȭ�� �ʺ� ����
	Settings::Get().SetHeigh(static_cast<float>(Window::GetHeight()));		// SettingŬ������ Heigh�� ȭ�� ���̸� ����

	// Windowâ�� �������ϰų� ������Ʈ�ϴ� ����� �������� ����
	std::unique_ptr<Engine> engine = std::make_unique<Engine>();

	while (Window::Update())
	{
		engine->Update();
		engine->Render();
	}

	// ������ �ı�
	Window::Destroy();
}