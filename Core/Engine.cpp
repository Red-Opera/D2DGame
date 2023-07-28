#include "stdafx.h"
#include "Engine.h"

Engine::Engine()
{
	context = new Context();
	context->RegisterSubSystem<Timer>();					// 시간관련 서브 시스템
	context->RegisterSubSystem<InputManager>();				// 키보드 및 마우스 입력 관련 서브시스템

	context->RegisterSubSystem<Graphics>();					// 그래픽에 관련 서브시스템
	context->RegisterSubSystem<SceneManager>();				// 장면 관련 서브시스템

	context->RegisterSubSystem<Renderer>();					// Render함수 관련 서브시스템
	context->RegisterSubSystem<ResourceManager>();			// 리소스 관련 서브시스템

	context->RegisterSubSystem<ImGuiControl>();				// ImGui 관련 서브시스템
	context->RegisterSubSystem<GetCursorSystem>();			// 커서관련 서브 시스템
	context->RegisterSubSystem<ChangeAttackTypeManager>();	// 공격변화 관련 서브시스템
	context->RegisterSubSystem<PressUI>();					// UI관련 서브시스템
	context->RegisterSubSystem<ScoreUI>();

	bool hr = context->InitializeSubSystems();

	if (hr) LOG_INFO("서브 시스템 초기화 성공");
	else	LOG_ERROR("서브 시스템 초기화 실패");

  //LOG_TARGET_FILE();
  //LOG_WARNING("플레이어가 비정상적인 위치에 도달함 : %d, %d, %d", 10, 20, 30);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	sizeof(io);

	// Setup Dear ImGui style
  //ImGui::StyleColorsDark();
  //ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
   //ImGui_ImplWin32_Init(Settings::Get().GetWindowHandle());

	auto graphics = context->GetSubSystem<Graphics>();
    ImGui_ImplDX11_Init(graphics->GetDevice(), graphics->GetDeviceContext());

	// 폰트 관련
  //ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\HANBatang.ttf", 16.0f, NULL, ImGui::GetIO().Fonts->GetGlyphRangesKorean());
  //ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\CascadiaCode.ttf", 15.0f, NULL, ImGui::GetIO().Fonts->GetGlyphRangesKorean());
}

Engine::~Engine()
{
	SAFE_DELETE(context);
}

void Engine::Update()
{
	// Start the Dear ImGui frame
  //ImGui_ImplDX11_NewFrame();
  //ImGui_ImplWin32_NewFrame();
  //ImGui::NewFrame();

	context->UpdateSubSystems();
}

void Engine::Render()
{
	Renderer* renderer = context->GetSubSystem<Renderer>();
	renderer->Update();
}
