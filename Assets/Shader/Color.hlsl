// Color.hlsl
// 정점을 색깔입히기 위한 파일

// IA에서 받은 정보를 담는 구조체
struct VertexInput
{
	/*
		POSITION0 -> Sementic
		
		Sementic
			- 데이터의 출처와 역할에 대한 의미를 부여하기 위한 키워드 (해당 키워드는 사용자가 정의하는 것이 아니라 이미 정해져 있음)
			- POSITON0에서 POSITION은 이름, 0은 인덱스이다.
	*/

	float4 position : POSITION0;	// float4 == float[4]
	float4 color : COLOR0;
};

// Pixel Shader를 PS단계로 보내기 위한 구조체
struct PixelInput
{
	/*
		System Value(SV)
			- 시스템에서 사용할 값이라고 명시한 것
			- SV_ 접두사를 통해 해당 값이 시스템에서 특별한 의미가 있다고 표현한 것이다.
			- SV는 랜더링 파이프 라인의 단계에 따라 의미가 다를 수 있다.
			- 해당 접두사로 시스템이 사용할 값을 지정해주거나, 시스템에서 어떠한 의미를 가진 값을 받을 수 있다.
			- SV_가 붙는 다양한 키워드가 있고 이건 문서에서 확인
			  (링크 : https://docs.microsoft.com/ko-kr/windows/win32/direct3dhlsl/dx-graphics-hlsl-semantics?redirectedfrom=MSDN)
			- VS단계에서의 SV_POSITION의 경우는 해당 SV를 가진 데이터를 첨부하여 출력할 경우(PixelInput VS함수에서 return output),
			  해당 값은 RS단계에서 레스터 이미지화를 할 때 사용할, VS단계에서 최종 변환과정을 거쳐서 나온 정점이라는 의미를 시스템에게 전달할 수 있다.
			- PS단계에서의 SV_POSITION의 경우는 VS단계에서 넘긴 SV_POSTION의 값을 PS 단계가 오기 전인
			  RS단계를 거치면서 완성된 래스터 이미지의 픽셀 좌표들을 받겠다는 의미이다.

		래스터 이미지 : 픽셀로 이미지를 표현하는 자료구조, 비트맵이라고도 부른다.
	*/

	float4 position : SV_POSITION0;	// RS단계에서 픽셀화 할 때 사용하라는 뜻
	float4 color : COLOR0;
};

/*
	- cbuffer : Constant Buffer의 약자
	- register : 세이터에 데이터를 연결 시켜준다.
		1. b : buffer
		2. 0 : slot, 저장할 레지스터 슬롯의 번호, 0 ~ 13 범위(b0 ~ b13)를 가짐
	- matrix : 4x4 행렬


	상수 버퍼는 16Byte 단위로 정렬해야 한다.
	최대 4096 * 16Byte만큼 받을 수 있다.
*/

cbuffer CameraBuffer : register(b0)
{
    matrix view;
    matrix projection;
}

cbuffer TransformBuffer : register(b1)
{
    matrix world;
}

cbuffer ColorBuffer : register(b2)
{
    float4 color;
}

// VS 단계 : IA에서 받은 정점을 공간변환해주는 단계
PixelInput VS(VertexInput input)
{
	PixelInput output;

	/*
		1 x 4(float4 position) * 4 x 4(world) = 1 x 4
 	*/

	// 받은 포지션 값에 SV_접두사를 붙여준다. -> RS단계에서 사용할 정점을 지정해준 것이다.
	// output.position = input.position;

	// 행렬 곱을 통해 공간이동을 시켜준다.
	output.position = mul(input.position, world);		// world공간으로 변환된 값을 데이터에 저장
	output.position = mul(output.position, view);		// view공간으로 변환된 값을 데이터에 저장
	output.position = mul(output.position, projection); // projection공간으로 변환된 값을 데이터에 저장 (Direct X에서는 Clip공간도 동시에 해줌)
	output.color = input.color;

	return output;	// 반환된 값은 RS단계를 거치게 된다.
}

// SV_Target : 현재 세팅된 메인 렌더 타겟을 사용해라
float4 PS(PixelInput input) : SV_Target // 완성된 색상이 어디로 가야하는지 알려주지 않으면 예외가 발생한다.
{
    return input.color;
}