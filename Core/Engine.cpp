#include "stdafx.h"
#include "Engine.h"

Engine::Engine()
{
	context = new Context();
	context->RegisterSubSystem<Timer>();					// �ð����� ���� �ý���
	context->RegisterSubSystem<InputManager>();				// Ű���� �� ���콺 �Է� ���� ����ý���

	context->RegisterSubSystem<Graphics>();					// �׷��ȿ� ���� ����ý���
	context->RegisterSubSystem<SceneManager>();				// ��� ���� ����ý���

	context->RegisterSubSystem<Renderer>();					// Render�Լ� ���� ����ý���
	context->RegisterSubSystem<ResourceManager>();			// ���ҽ� ���� ����ý���

	context->RegisterSubSystem<ImGuiControl>();				// ImGui ���� ����ý���
	context->RegisterSubSystem<GetCursorSystem>();			// Ŀ������ ���� �ý���
	context->RegisterSubSystem<ChangeAttackTypeManager>();	// ���ݺ�ȭ ���� ����ý���
	context->RegisterSubSystem<PressUI>();					// UI���� ����ý���
	context->RegisterSubSystem<ScoreUI>();

	bool hr = context->InitializeSubSystems();

	if (hr) LOG_INFO("���� �ý��� �ʱ�ȭ ����");
	else	LOG_ERROR("���� �ý��� �ʱ�ȭ ����");

  //LOG_TARGET_FILE();
  //LOG_WARNING("�÷��̾ ���������� ��ġ�� ������ : %d, %d, %d", 10, 20, 30);

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

	// ��Ʈ ����
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
