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

	// ISubSystem을(를) 통해 상속됨
	virtual bool Initialize() override;
	virtual void Update() override;

	// 객체의 Transform 출력
	static bool OutputTransform
	(
		std::shared_ptr<class Actor> OutputTarget,		// 데이터 출력할 객체
		const std::string title,						// 출력할 제목
		FigureType posType = FigureType::None, FigureType scaleType = FigureType::None, 					// 위치 정보 출력타입, 크기 정보 출력 타입
		float fMinValue = 0.0f, float fMaxValue = 100.0f, float sMinValue = 0.0f, float sMaxValue = 100.0f	// 첫번째 슬라이더 최소값, 첫번째 슬라이더 최대값, 두번째 슬라이더 최소값, 두번째 슬라이더 최대값
	);

private:
	static void Float3FigureTypeProcess(D3DXVECTOR3& data, const FigureType& figureType, std::string&& name, float min = 0.0f, float max = 0.0f);	// 출력 타입에 맞춰 출력
};