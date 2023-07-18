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
	// 둘다 FigureType이 None인 경우 오류 발생
	if ((posType == FigureType::None) && (scaleType == FigureType::None))
	{
		assert(false && "ERROR (ImGui1) : 위치 또는 크기 출력 타입 중 하나 이상 지정 필요");
		return false;
	}

	if (ImGui::Begin((OutputTarget.get()->GetName() + " " + title).c_str())) // 객체이름 Transform
	{
		// 위치 정보 출력
		if (posType != FigureType::None)
		{
			D3DXVECTOR3 pos = OutputTarget->GetComponent<TransformComponent>()->GetPosition();		// 객체로 부터 위치 정보를 받아 옴

			if (pos == nullptr)
			{
				assert(false && "ERROR (ImGui2) : 입력한 객체로부터 위치 정보를 찾을 수 없음");
			}

			switch (posType)
			{
			case FigureType::Input3:				// 위치 정보를 Input으로 출력함
				Float3FigureTypeProcess(pos, posType, " Position");

				OutputTarget->GetComponent<TransformComponent>()->SetPosition(pos);
				break;

			case FigureType::Slider3:				// 위치 정보를 Slider로 출력함
			{
				Float3FigureTypeProcess(pos, posType, "Position", fMinValue, fMaxValue);

				OutputTarget->GetComponent<TransformComponent>()->SetPosition(pos);
				break;
			}

			default:
				break;
			}
		}

		// 크기 정보 출력
		if (scaleType != FigureType::None)
		{
			D3DXVECTOR3 scale = OutputTarget->GetComponent<TransformComponent>()->GetScale();	// 객체로 부터 크기 정보를 받아 옴

			switch (scaleType)
			{
			case FigureType::Input3:			// 크기 정보를 Input로 출력함
				Float3FigureTypeProcess(scale, scaleType, "Scale");

				OutputTarget->GetComponent<TransformComponent>()->SetScale(scale);
				break;

			case FigureType::Slider3:			// 크기 정보를 Slider로 출력함
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