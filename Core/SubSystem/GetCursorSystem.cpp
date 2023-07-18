#include "stdafx.h"
#include "GetCursorSystem.h"

bool GetCursorSystem::Initialize()
{
	return true;
}

void GetCursorSystem::Update()
{
	int windowPosX, windowPosY;

	GetWindowPos(windowPosX, windowPosY);

	POINT cursorPos;
	GetCursorPos(&cursorPos);

	GetCursorSystem::mouse = { cursorPos.x - windowPosX - static_cast<int>(Settings::Get().GetWidth() / 2), -(cursorPos.y - windowPosY - static_cast<int>(Settings::Get().GetHeight() / 2)) };
}

void GetCursorSystem::GetWindowPos(int& x, int& y)
{
	RECT rect;
	GetWindowRect(Settings::Get().GetWindowHandle(), &rect); // Ã¢ÀÇ »ç°¢Çü °æ°è¸¦ °¡Á®¿È

	x = rect.left;	// Ã¢ÀÇ X ÁÂÇ¥
	y = rect.top;	// Ã¢ÀÇ Y ÁÂÇ¥
}