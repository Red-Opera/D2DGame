#pragma once
#include "ISubSystem.h"

enum class FigureType
{
	None, Input3, Slider3
};


class ImGuiControl : public ISubSystem
{
public:
	using ISubSystem::ISubSystem;
	~ImGuiControl() = default;

	// ISubSystem��(��) ���� ��ӵ�
	virtual bool Initialize() override;
	virtual void Update() override;

	// ��ü�� Transform ���
	static bool OutputTransform
	(
		std::shared_ptr<class Actor> OutputTarget,		// ������ ����� ��ü
		const std::string title,						// ����� ����
		FigureType posType = FigureType::None, FigureType scaleType = FigureType::None, 					// ��ġ ���� ���Ÿ��, ũ�� ���� ��� Ÿ��
		float fMinValue = 0.0f, float fMaxValue = 100.0f, float sMinValue = 0.0f, float sMaxValue = 100.0f	// ù��° �����̴� �ּҰ�, ù��° �����̴� �ִ밪, �ι�° �����̴� �ּҰ�, �ι�° �����̴� �ִ밪
	);

private:
	static void Float3FigureTypeProcess(D3DXVECTOR3& data, const FigureType& figureType, std::string&& name, float min = 0.0f, float max = 0.0f);	// ��� Ÿ�Կ� ���� ���
};