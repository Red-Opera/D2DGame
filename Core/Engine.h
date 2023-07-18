// Engine.h
// ��� �Ŵ��� ����ϰ� ��������ִ� ����

#pragma once

class Engine
{
public:
	Engine();
	~Engine();

	void Update();
	void Render();

private:
	class Context* context = nullptr;	// �Ŵ����� ����ϰ� �����ϱ� ���� ����
};