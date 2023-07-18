// Renderer_Resource.cpp

#include "stdafx.h"

void Renderer::CreateConstantBuffers()
{
    // ��� ���� �ʱ�ȭ
    gpu_camera_buffer = std::make_shared<D3D11_ConstantBuffer>(graphics);
    gpu_camera_buffer->Create<CAMERA_DATA>();

    gpu_object_buffer = std::make_shared<D3D11_ConstantBuffer>(graphics);
    gpu_object_buffer->Create<TRANSFORM_DATA>();

    gpu_animation_buffer = std::make_shared<D3D11_ConstantBuffer>(graphics);
    gpu_animation_buffer->Create<ANIMATION_DATA>();
}

void Renderer::CreateRasterizerStates()
{
    // RasterizerState���� ����� �� �ִ� ��� ���¸� Ű�� ����� �ش� Ű�� RasterizerState ��ü�� ������
    for (unsigned i = 0; i <= static_cast<uint>(RasterizerStateType::Max); i++)
        rasterizers[static_cast<RasterizerStateType>(i)] = std::make_shared<D3D11_RasterizerState>(graphics);

    // ������ Ű�� �ش��ϴ� ���¸� Create�� ������
    rasterizers[RasterizerStateType::Cull_Front_Fill_Solid]->Create(D3D11_CULL_FRONT, D3D11_FILL_SOLID);
    rasterizers[RasterizerStateType::Cull_Back_Fill_Solid]->Create(D3D11_CULL_BACK, D3D11_FILL_SOLID);
    rasterizers[RasterizerStateType::Cull_None_Fill_Solid]->Create(D3D11_CULL_NONE, D3D11_FILL_SOLID);
    rasterizers[RasterizerStateType::Cull_Front_Fill_Wirefame]->Create(D3D11_CULL_FRONT, D3D11_FILL_WIREFRAME);
    rasterizers[RasterizerStateType::Cull_Back_Fill_Wirefame]->Create(D3D11_CULL_BACK, D3D11_FILL_WIREFRAME);
    rasterizers[RasterizerStateType::Cull_None_Fill_Wirefame]->Create(D3D11_CULL_NONE, D3D11_FILL_WIREFRAME);
}

void Renderer::CreateBlendStates()
{
    // BlendState���� ����� �� �ִ� ��� ���¸� Ű�� ����� �ش� Ű�� BlendState ��ü�� ������
    for (unsigned i = 0; i <= static_cast<uint>(BlendStateType::Max); i++)
        blend_states[static_cast<BlendStateType>(i)] = std::make_shared<D3D11_Blend_state>(graphics);

    // ������ Ű�� �ش��ϴ� ���¸� Create�� ������
    blend_states[BlendStateType::Disabled]->Create(false);
    blend_states[BlendStateType::Alpha]->Create(true);
}