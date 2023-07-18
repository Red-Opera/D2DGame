// Graphics.cpp

#include "stdafx.h"
#include "Graphics.h"

Graphics::~Graphics()
{
	// 프로그램이 종료될 쯤 생성해준 모든 자원을 해제함, 생성된 역순으로 지워줌
	SAFE_RELEASE(render_target_view);
	SAFE_RELEASE(device);
	SAFE_RELEASE(device_context);
	SAFE_RELEASE(swap_chain);
}

// 그래픽 작업을 할 때 가장 먼저 처리하는 함수
bool Graphics::Initialize()
{
	DXGI_SWAP_CHAIN_DESC desc;							// DXGI_SWAP_CHAIN_DESC : 스왑 체인을 만들기 위한 설명서
	ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC));

	// 스왑체인이 관리할 버퍼의 크기 (백버퍼의 크기)
	desc.BufferDesc.Width = 0;							// 너비
	desc.BufferDesc.Height = 0;							// 높이

	// RefreshRate : 화면 주사율, 화면의 갱신주기, 초당 "Numerator/Denominator"만큼 갱신 (반환값 : unsigned int -> 양의 정수형)
	desc.BufferDesc.RefreshRate.Numerator = 60;			// 분자
	desc.BufferDesc.RefreshRate.Denominator = 1;		// 분모	

	/*
		- RGBA
			1. 색상 채널을 의미
			2. 무조건 색상 데이터로 담을 필요는 없다. -> 즉, 색상이 아닌 좌표의 정보를 담는 데이터 형식으로도 사용할 수 있다. (ex : R32G32 -> int형 좌표값)
			3. R8G8B8A8 : BPC, ColorDepth라고 부르며 각각 채널이 얼마의 메모리를 할당할지 의미하기도 한다.
			   R32G32 : 4바이트를 가지는 자료를 2개를 가지겠다는 뜻

		- UNORM(Unsigned Normalize) : 정규화
			1. 0 ~ 1로 정규화된 데이터를 사용
			2. 각 채널이 8비트인 상태에서 정규화를 할 경우 0 ~ 255 범위를 0 ~ 1인 상태로 바꾼다는 뜻

		DXGI_FORMAT_R8G8B8A8_UNORM -> 4바이트 크기(각 채널당 8비트 * 4)를 가지는 정규화된 데이터를 담겠다.
	*/

	// 버퍼의 디스플레이 색상 형식
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // RGBA색상을 8비트(1바이트)씩 할당하겠다는 뜻 -> 각 채널이 8비트이니 채널별로 0 ~ 255의 범위를 가진다.

	/*
		ScanlineOrdering
		
		- 화면을 출력할 때 픽셀을 어떤 순서대로 한 줄씩 출력하는지 지정하는 변수
		- Scanline : 한 줄씩 출력
		- 목록 
			1. UNSPECIFIED : 지정하지 않음
			2. PROGRESSIVE : 위에서 아래로 차례대로 출력
			3. UPPER_FIELD_FIRST : 홀수번째 줄 먼저 출력하고 짝수번째 줄 출력
			4. LOWER_FIELD_FIRST : 짝수번째 줄 먼저 출력하고 홀수번째 줄 출력
	*/

	desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; // 일반적으로 화면 출력

	// 화면을 축소/확대 할 때 백버퍼의 화면 비율 차이 처리방식
	desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	desc.BufferCount = 1;								// 버퍼의 개수
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// 버퍼의 용도 또는 방식 (DXGI_USAGE_RENDER_TARGET_OUTPUT : 출력 랜더링 할 대상, 백버퍼를 그리는 용도)

	/*
		- Aliasing : 픽셀 표현으로 인해 확대하면 외곽선이 계단처럼 보이는 현상
		- Anti Aliasing(AA) - 계단 현상을 제거하는 방법

			1. Super Sampling(SSAA)
			- 샘플링 배수만큼 해당도를 확대한 후에 Down Sampling(해상도를 줄여 색을 섞는다)을 하는 방식
			- 많은 연산량과 메모리가 필요하다.

			2. Multi Sampling AA(MSAA : 다중 표본화 안티엘리언싱)
			- 많은 연산량과 메모리를 요구하는 SSAA를 개량해서 만든 방법
			- 폴리곤(이미지)의 가장자리만 AA을 진행한다.

		// 과제 : 안티 앨리어싱 조사하기
		// 과제 : BPC 조사
	*/

	desc.SampleDesc.Count = 1;					// 안티엘리언싱의 샘플 개수, 배수(얼마만큼 늘릴 것인가)
	desc.SampleDesc.Quality = 0;				// 샘플링의 퀄리티 수준, 0은 안쓰겠다는 의미

	// 윈도우 관련 옵션
	desc.OutputWindow = Settings::Get().GetWindowHandle();	// 출력할 윈도우 핸들 (입력값 : Window 핸들)
	desc.Windowed = TRUE;									// 창모드 여부 (FALSE로 할 경우 일반적인 게임처럼 화면이 전체화면이 되고 최대축소 및 종료 메뉴가 없어진다.)

	// 백버퍼와 스왑시 어떤 효과를 줄 것인가?
	desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; // DISCARD : 스왑하여 다른 데이터를 폐기한다.

	// FEATURE_LEVEL : 기능 수준, DX 어느 버전을 쓸 것인가?
	std::vector<D3D_FEATURE_LEVEL> feature_levels
	{
	   D3D_FEATURE_LEVEL_11_1,
	   D3D_FEATURE_LEVEL_11_0,
	   D3D_FEATURE_LEVEL_10_1,
	   D3D_FEATURE_LEVEL_10_0,
	};

	/*
		D3D11CreateDeviceAndSwapChain : 디바이스와 스왑체인을 만들어주는 함수
			- 반환값 : HRESULT -> 함수 결과의 성공여부를 담는 자료형 (S_OK(성공) 또는 S_FALSE(실패)의 값을 가지는 데이터형)
	*/

	HRESULT hr = D3D11CreateDeviceAndSwapChain
	(
		nullptr,					// 비디오 카드(그래픽 카드)를 저정해서 사용할 때 쓴다
		D3D_DRIVER_TYPE_HARDWARE,	// 디바이스 타입 (D3D_DRIVER_TYPE_SOFRWARE : 랜더링 할 떄 소프트웨어로 할 경우 -> 테스트 중 랜더링할 때 하드웨어의 성능이 좋지 않는 다양한 환경에서 사용할 때 사용)
		nullptr,					// 다른 소프트웨어를 추가적으로 넣을 것인가?
		0,							// 디바이스를 만들 때 어떤 기능을 추가적으로 넣을 것인가?
		feature_levels.data(),		// 스왑체인이 사용할 버전, 피처 레벨(FEATURE_LEVEL) 배열의 시작 주소
		feature_levels.size(),		// 피처 레벨 카운트(배열의 배수)
		D3D11_SDK_VERSION,			// 사용하는 SDK 버전
		&desc,						// 스왑체인을 만들 때 사용하는 desc 주소
		&swap_chain,				// 생성된 스왑체인을 할당받을 포인터 변수의 주소
		&device,					// 생성된 디바이스를 할당받을 포인터 변수의 수소
		nullptr,					// 선택된 피처레벨을 반환받을 변수의 주소 (선택된 DX버전을 반환함)
		&device_context				// 생성된 디바이스 컨택스트를 할당받을 포인터 변수의 주소
	);

	assert(SUCCEEDED(hr));			// SUCCEEDED() : HRESULT변수 결과값이 성공시 true, false를 반환해주는 매크로

	return true;
}

void Graphics::Update()
{
	
}

void Graphics::CreateBackBuffer(const uint& width, const uint& height)
{
	// ===============================================================
	//						백버퍼 수정 영역
	// ===============================================================
	/*
		ResizeBuffers
			- 스왑 체인이 가지는 버퍼 정보를 변경하는 함수
			- 변경할 값만 넣어주고 변경하지 않을 값을 초기값을 넣으면 변경이 되지 않는다.
	*/

	// 스왑체인에 있는 BufferDesc.Width와 BufferDesc.Height을 변경하기 위해서 swap_chain의 ResizeBuffer함수를 호출함
	HRESULT hr = swap_chain->ResizeBuffers
	(
		0,						// 버퍼의 개수	(변경X)
		width,					// 너비
		height,					// 높이
		DXGI_FORMAT_UNKNOWN,	// 버퍼의 형식	(변경X)
		0						// 플래그		(변경X)
	);
	assert(SUCCEEDED(hr));

	// ===============================================================
	//					RTV(render_target_view) 생성
	// ===============================================================
	/*
		Interface Idenrifier(IID)
			- 인테페이스를 구분하기 위한 식별자
			- GUID의 값 중 하나

		Globally Unique Identifier(GUID)
			- MS에서 사용하는 여러 항목을 구분하기 위한 식별자
			- 16바이트의 크기를 가진다. (난수로 해서 값을 뽑는다.)
			- U(Universal)UID라고도 있는데 큰 차이는 없다.

		과제 : GUID, UUID 조사
	*/

	// 스왑체인에 있는 버퍼를 가져오는 부분
	ID3D11Texture2D* back_buffer = nullptr;		// 버퍼를 2D데이터 값으로 가져옴

	hr = swap_chain->GetBuffer
	(
		0,										// 스왑체인의 몇번째의 버퍼를 꺼낼 것인가?
		__uuidof(ID3D11Texture2D),				// 백버퍼가 받을 반환형 UUID값, __uuidof : 자료형마다 존재하고 고유값인 UUID값을 가져오는 키워드
		reinterpret_cast<void**>(&back_buffer)	// 반환된 인터페이스를 받을 저장공간, void** : Texture 3D또는 1D일 수도 있기 때문
	);
	assert(SUCCEEDED(hr));

	// render_target_view를 만드는 부분
	hr = device->CreateRenderTargetView			// buffer에 저장된 내용을 기반으로 render_target_view에 값을 저장함
	(
		back_buffer,							// 어떤 자원을 이욯해서 뷰에서 접근할 리소스
		nullptr,								// RTV의 desc, nullptr인 경우 리소스의 형식을 따른다.
		&render_target_view						// RTV를 받아올 변수 -> 해당 위치에서 back_buffer의 값을 넘겨줬으므로 back_buffer는 해제해줘도 된다.
	);
	assert(SUCCEEDED(hr));


	// ===============================================================
	//			뷰포트(그림을 그릴 영역) 세팅 -> WIndow창 크기를 잡음
	// ===============================================================
	viewport.x = 0.0f;						// 가장 오른쪽 위의 x좌표
	viewport.y = 0.0f;						// 가장 오른쪽 위의 y좌표
	viewport.width = static_cast<float>(width);		// Window창의 너비
	viewport.height = static_cast<float>(height);	// Window창의 높이
	viewport.min_depth = 0.0f;						// 3D관련 최소 깊이
	viewport.max_depth = 1.0f;						// 3D관련 최대 깊이 (크기를 0으로 하면 없는것이 되기 때문에 2D여도 최대와 최소의 깊이는 같으면 안된다.)

	// 해제
	SAFE_RELEASE(back_buffer);
}

// Graphics의 Begin함수의 내용을 Execute의 중간단계에 작성해도 되지만 논리상 작성
void Graphics::Begin()
{
	/*
		 Output Merger Stage(OM) - 출력 병합 단계
		 - 그림을 그릴 렌더 타겟을 지정하고, 색을 어떻게 섞을 것인가(블렌딩)을 지정해준다.
		 - OM에 세팅 될 수 있는 렌더 타겟의 개수는 8개이다. -> 8장 동시에 그릴 수 있다.
	*/

	device_context->OMSetRenderTargets(1, &render_target_view, nullptr);	// 그림을 그릴 렌터타겟을 지정

	D3D11_VIEWPORT d3d11_viewport;

	d3d11_viewport.TopLeftX = viewport.x;
	d3d11_viewport.TopLeftY = viewport.y;
	d3d11_viewport.Width = viewport.width;
	d3d11_viewport.Height = viewport.height;
	d3d11_viewport.MinDepth = viewport.min_depth;
	d3d11_viewport.MaxDepth = viewport.max_depth;

	device_context->RSSetViewports(1, &d3d11_viewport);						// 그릴 영역 선택
	device_context->ClearRenderTargetView(render_target_view, clear_color);	// 렌더 타겟(도화지) 초기화
}

void Graphics::End()
{
	// 더블 버퍼링을 관리하는 swap_chain에 있는 Present를 이용하여 화면 전면으로 보냄
	HRESULT hr = swap_chain->Present(1, 0); // 수직 동기화 값, 플래그 값
	assert(SUCCEEDED(hr));
}