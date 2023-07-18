#include "stdafx.h"
#include "Scene/Actor.h"
#include "Scene/Scene.h"
#include "Scene/Component/CameraComponent.h"
#include "Scene/Component/MeshRendererComponent.h"

Renderer::~Renderer()
{
}

bool Renderer::Initialize()
{
    // Graphics 생성
    graphics = context->GetSubSystem<Graphics>();
    graphics->CreateBackBuffer(static_cast<uint>(Settings::Get().GetWidth()), static_cast<uint>(Settings::Get().GetHeight()));

    // PipeLine 생성
    pipeline = std::make_shared<D3D11_Pipeline>(graphics);

    CreateConstantBuffers();
    CreateRasterizerStates();
    CreateBlendStates();

    return true;
}

void Renderer::Update()
{
    // 카메라가 있을 경우에만 실행하도록 설정
    if (camera == nullptr) return;

    D3DXMatrixTranspose(&cpu_camera_buffer.view, &camera->GetViewMatrix());
    D3DXMatrixTranspose(&cpu_camera_buffer.projection, &camera->GetProjectionMatrix());
    UpdateCameraBuffer();

    graphics->Begin();
    PassMain();
    graphics->End();
}

void Renderer::UpdateRenderables(Scene* const scene)
{
    // 기본 초기화
    renderables[RenderableType::Opaque].clear();
    renderables[RenderableType::Camera].clear();

    auto actors = scene->GetActors();
    if (actors.empty()) return;

    for (const auto& actor : actors)
    {
        auto camera_compoenent = actor->GetComponent<CameraComponent>();            
        auto mesh_renderer_compoenet = actor->GetComponent<MeshRendererComponent>();

        // 카메라가 있다면 카메라를 렌더한다.
        if (camera_compoenent != nullptr)
        {
            renderables[RenderableType::Camera].emplace_back(actor.get());
            camera = camera_compoenent.get();   // 카메라가 하나밖에 없으니 바로 메인카메라로 설정
        }

        // Mesh 랜더가 있다면 랜더링을 한다.
        if (mesh_renderer_compoenet != nullptr)
        {
            renderables[RenderableType::Opaque].emplace_back(actor.get());
        }
    }
}

void Renderer::UpdateCameraBuffer()
{
    auto buffer = gpu_camera_buffer->Map<CAMERA_DATA>();
    *buffer = cpu_camera_buffer;
    gpu_camera_buffer->Unmap();
}

void Renderer::UpdateObjectBuffer()
{
    auto buffer = gpu_object_buffer->Map<TRANSFORM_DATA>();
    *buffer = cpu_object_buffer;
    gpu_object_buffer->Unmap();
}

void Renderer::UpdateAnimationBuffer()
{
    auto buffer = gpu_animation_buffer->Map<ANIMATION_DATA>();
    *buffer = cpu_animation_buffer;
    gpu_animation_buffer->Unmap();
}