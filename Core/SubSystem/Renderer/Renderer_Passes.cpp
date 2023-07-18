// Renderer_Passes.cpp
// Rederer.h�� �ִ� PassMain�� �и���Ű�� ����

#include "stdafx.h"
#include "Scene/Actor.h"
#include "Scene/Component/MeshRendererComponent.h"
#include "Scene/Component/MoveComponent/TransformComponent.h"
#include "Scene/Component/AnimationComponent/AnimatorComponent.h"
#include "Scene/Component/AnimationComponent/BackGroundManager.h"

void Renderer::PassMain()
{
	std::vector<Actor*>& actors = renderables[RenderableType::Opaque];

	if (actors.empty()) return;

	for (const auto& actor : actors)
	{
		auto renderable = actor->GetComponent<MeshRendererComponent>();

		if (!actor->IsActive()) continue;

		// MeshRendererComponent�� �ִ��� Ȯ��
		if (renderable == nullptr || !renderable->IsEnabled()) continue;

		auto transform = actor->GetTransform_Raw();
		if (transform == nullptr) continue;

		// ���������� ����
		D3D11_PipelineState pipeline_state;

		pipeline_state.input_layout = renderable->GetInputLayout().get();
		pipeline_state.primitive_topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		pipeline_state.vertex_shader = renderable->GetVertexShader().get();
		pipeline_state.pixel_shader = renderable->GetPixelShader().get();
		pipeline_state.rasterizer_state = rasterizers[RasterizerStateType::Cull_Back_Fill_Solid].get();
		pipeline_state.blend_state = blend_states[BlendStateType::Alpha].get();

		if (pipeline->Begin(pipeline_state))
		{
			pipeline->SetVertexBuffer(renderable->GetVertexBuffer().get());
			pipeline->SetIndexBuffer(renderable->GetIndexBuffer().get());

			D3DXMatrixTranspose(&cpu_object_buffer.world, &transform->GetWorldMatrix());
			UpdateObjectBuffer();

			// hlsl�� cbuffer�� ���� camera�� ��ü�� ��� ���۸� ����
			pipeline->SetConstantBuffer(0, ShaderScope_VS, gpu_camera_buffer.get());
			pipeline->SetConstantBuffer(1, ShaderScope_VS, gpu_object_buffer.get());

			// AnimationComponent�� �ٿ��� actor �ؽ��ĸ� cpu buffer�� ������ �ѱ�
			if (auto animator = actor->GetComponent<AnimatorComponent>())
			{
				auto current_keyframe = animator->GetCurrentKeyframe();
				cpu_animation_buffer.sprite_offset = current_keyframe->offset;
				cpu_animation_buffer.sprite_size = current_keyframe->size;
				cpu_animation_buffer.texture_size = animator->GetCurrentAnimation()->GetSpriteTextureSize(animator->GetNowKeyFrameName());
				cpu_animation_buffer.isTexture = 1.0f;
				UpdateAnimationBuffer();

				// VS�� PS�� ó���ϱ� ���� animation�� ��� ���۸� hlsl�� ����
				pipeline->SetConstantBuffer(2, ShaderScope_VS | ShaderScope_PS, gpu_animation_buffer.get());
				// ShaderResource�� ���ؼ� hlsl�� source_texture�� ����
				pipeline->SetShaderResource(0, ShaderScope_PS, animator->GetCurrentAnimation()->GetSpriteTexture().get());
			}

			// �������� �ʴ� �ؽ����� ���
			else if (auto texture = actor->GetComponent<BackGroundComponent>())
			{
				cpu_animation_buffer.sprite_offset = texture->GetTexture()->GetSpriteTextureOffset();

				if (texture->GetTexture()->IsFullSize())
					cpu_animation_buffer.sprite_size = D3DXVECTOR2(Settings::Get().GetWidth(), Settings::Get().GetHeight());
				else cpu_animation_buffer.sprite_size = texture->GetTexture()->GetSpriteTextureSize();

				cpu_animation_buffer.texture_size = texture->GetTexture()->GetSpriteTextureSize();
				cpu_animation_buffer.isTexture = 1.0f;
				UpdateAnimationBuffer();

				pipeline->SetConstantBuffer(2, ShaderScope_VS | ShaderScope_PS, gpu_animation_buffer.get());
				pipeline->SetShaderResource(0, ShaderScope_PS, texture->GetTexture()->GetSpriteTexture().get());
			}

			// �ؽ��ĸ� �־����� ���� ���
			else
			{
				cpu_animation_buffer.sprite_offset = D3DXVECTOR2(0, 0);
				cpu_animation_buffer.sprite_size = D3DXVECTOR2(1, 1);	
				cpu_animation_buffer.texture_size = D3DXVECTOR2(1, 1);
				cpu_animation_buffer.isTexture = 0.0f;
				UpdateAnimationBuffer();

				pipeline->SetConstantBuffer_nullptr(2, ShaderScope_VS | ShaderScope_PS);
				pipeline->SetShaderResource_nullptr(0, ShaderScope_PS);
			}

			pipeline->DrawIndexed(renderable->GetIndexBuffer()->GetCount(),
							      renderable->GetIndexBuffer()->GetOffset(),
								  renderable->GetVertexBuffer()->GetOffset());

			pipeline->End();
		}
	}

	bool show_demo_window = true;

	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	// Rendering
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}