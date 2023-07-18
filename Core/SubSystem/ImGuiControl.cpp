#include "stdafx.h"
#include "ImGuiControl.h"

#include "Scene/Actor.h"
#include "Scene/Component/MoveComponent/TransformComponent.h"

bool ImGuiControl::Initialize()
{
	return true;
}

void ImGuiControl::Update()
{
}

void ImGuiControl::Float3FigureTypeProcess(D3DXVECTOR3& data, const FigureType& figureType, std::string&& name, float min, float max)
{
	switch (figureType)
	{
	case FigureType::Input3:
		ImGui::InputFloat3(name.c_str(), data);
		break;

	case FigureType::Slider3:
		ImGui::SliderFloat3(name.c_str(), data, min, max, "%.1f");
		break;

	default:
		break;
	}
}

bool ImGuiControl::OutputTransform(std::shared_ptr<class Actor> OutputTarget, const std::string title, FigureType posType, FigureType scaleType, float fMinValue, float fMaxValue, float sMinValue, float sMaxValue)
{
	// �Ѵ� FigureType�� None�� ��� ���� �߻�
	if ((posType == FigureType::None) && (scaleType == FigureType::None))
	{
		assert(false && "ERROR (ImGui1) : ��ġ �Ǵ� ũ�� ��� Ÿ�� �� �ϳ� �̻� ���� �ʿ�");
		return false;
	}

	if (ImGui::Begin((OutputTarget.get()->GetName() + " " + title).c_str())) // ��ü�̸� Transform
	{
		// ��ġ ���� ���
		if (posType != FigureType::None)
		{
			D3DXVECTOR3 pos = OutputTarget->GetComponent<TransformComponent>()->GetPosition();		// ��ü�� ���� ��ġ ������ �޾� ��

			if (pos == nullptr)
			{
				assert(false && "ERROR (ImGui2) : �Է��� ��ü�κ��� ��ġ ������ ã�� �� ����");
			}

			switch (posType)
			{
			case FigureType::Input3:				// ��ġ ������ Input���� �����
				Float3FigureTypeProcess(pos, posType, " Position");

				OutputTarget->GetComponent<TransformComponent>()->SetPosition(pos);
				break;

			case FigureType::Slider3:				// ��ġ ������ Slider�� �����
			{
				Float3FigureTypeProcess(pos, posType, "Position", fMinValue, fMaxValue);

				OutputTarget->GetComponent<TransformComponent>()->SetPosition(pos);
				break;
			}

			default:
				break;
			}
		}

		// ũ�� ���� ���
		if (scaleType != FigureType::None)
		{
			D3DXVECTOR3 scale = OutputTarget->GetComponent<TransformComponent>()->GetScale();	// ��ü�� ���� ũ�� ������ �޾� ��

			switch (scaleType)
			{
			case FigureType::Input3:			// ũ�� ������ Input�� �����
				Float3FigureTypeProcess(scale, scaleType, "Scale");

				OutputTarget->GetComponent<TransformComponent>()->SetScale(scale);
				break;

			case FigureType::Slider3:			// ũ�� ������ Slider�� �����
				if (posType == FigureType::Slider3) Float3FigureTypeProcess(scale, scaleType, "Scale", sMinValue, sMaxValue);
				else  Float3FigureTypeProcess(scale, scaleType, "Scale", fMinValue, fMaxValue);

				OutputTarget->GetComponent<TransformComponent>()->SetScale(scale);
				break;

			default:
				break;
			}
		}
	}
	ImGui::End();

	return true;
}