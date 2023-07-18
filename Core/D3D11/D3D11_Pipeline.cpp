#include "stdafx.h"
#include "D3D11_Pipeline.h"

D3D11_Pipeline::D3D11_Pipeline(Graphics* graphics)
{
    device_context = graphics->GetDeviceContext();
}

D3D11_Pipeline::~D3D11_Pipeline()
{
}

bool D3D11_Pipeline::Begin(const D3D11_PipelineState& pipeline_state)
{
    // ===============================================================
    //							InputLayout
    // ===============================================================
    {
        if (pipeline_state.input_layout != nullptr && pipeline_state.input_layout->GetResource() != nullptr)
        {
            if (bind_index_buffer_id != pipeline_state.input_layout->GetID())
            {
                device_context->IASetInputLayout(pipeline_state.input_layout->GetResource());
                bind_inputlayout_id = pipeline_state.input_layout->GetID();
            }
        }

        else
        {
            assert(false);
            return false;
        }
    }

    // ===============================================================
    //						 PrimitiveTopology
    // ===============================================================
    {
        if (bind_primitive_topology_id != pipeline_state.primitive_topology)
        {
            device_context->IASetPrimitiveTopology(pipeline_state.primitive_topology);
            bind_primitive_topology_id = static_cast<uint>(pipeline_state.primitive_topology);
        }
    }

    // ===============================================================
    //						    VertexShader
    // ===============================================================
    {
        if (pipeline_state.vertex_shader != nullptr && pipeline_state.vertex_shader->GetResource() != nullptr)
        {
            void* new_shader = pipeline_state.vertex_shader->GetResource();

            uint instance_count = 256;
            ID3D11ClassInstance* instance[256] = { nullptr };
            ID3D11VertexShader* current_shader = nullptr;
            device_context->VSGetShader(&current_shader, instance, &instance_count);

            if (current_shader != new_shader)
            {
                device_context->VSSetShader(static_cast<ID3D11VertexShader*>(new_shader), nullptr, 0);
            }
        }

        else
        {
            assert(false);
            return false;
        }
    }

    // ===============================================================
    //							PixelShader
    // ===============================================================
    {
        uint instance_count = 256;
        ID3D11ClassInstance* instance[256] = { nullptr };
        ID3D11PixelShader* current_shader = nullptr;
        device_context->PSGetShader(&current_shader, instance, &instance_count);

        void* new_shader = (pipeline_state.pixel_shader->GetResource() != nullptr) ? pipeline_state.pixel_shader->GetResource() : nullptr;
        if (current_shader != new_shader)
        {
            device_context->PSSetShader(static_cast<ID3D11PixelShader*>(new_shader), nullptr, 0);
        }
    }

    // ===============================================================
    //					      RasterizerState
    // ===============================================================
    {
        if (pipeline_state.rasterizer_state != nullptr && pipeline_state.rasterizer_state->GetResource() != nullptr)
        {
            if (bind_rasterizer_state_id != pipeline_state.rasterizer_state->GetID())
            {
                device_context->RSSetState(pipeline_state.rasterizer_state->GetResource());
                bind_rasterizer_state_id = pipeline_state.rasterizer_state->GetID();
            }
        }
    }

    // ===============================================================
    //							BlendState
    // ===============================================================
    {
        if (pipeline_state.blend_state != nullptr && pipeline_state.blend_state->GetResource() != nullptr)
        {
            if (bind_blend_state_id != pipeline_state.blend_state->GetID())
            {
                float factor = pipeline_state.blend_state->GetBlendFactor();
                float blend_factor[] = { factor, factor, factor, factor };
                uint sample_mask = pipeline_state.blend_state->GetSampleMask();

                device_context->OMSetBlendState(pipeline_state.blend_state->GetResource(), blend_factor, sample_mask);
                bind_blend_state_id = pipeline_state.blend_state->GetID();
            }
        }
    }

    return true;
}

void D3D11_Pipeline::End()
{
}

void D3D11_Pipeline::SetVertexBuffer(const D3D11_VertexBuffer* buffer)
{
    if (buffer == nullptr || buffer->GetResource() == nullptr)
    {
        assert(false);
        return;
    }

    if (bind_vertex_buffer_id == buffer->GetID()) return;

    ID3D11Buffer* buffers[]{ buffer->GetResource() };
    device_context->IASetVertexBuffers(0, 1, buffers, &buffer->GetStride(), &buffer->GetOffset());
    bind_vertex_buffer_id = buffer->GetID();
}

void D3D11_Pipeline::SetIndexBuffer(const D3D11_IndexBuffer* buffer)
{
    if (buffer == nullptr || buffer->GetResource() == nullptr)
    {
        assert(false);
        return;
    }

    if (bind_index_buffer_id == buffer->GetID()) return;

    device_context->IASetIndexBuffer(buffer->GetResource(), DXGI_FORMAT_R32_UINT, buffer->GetOffset());
    bind_index_buffer_id = buffer->GetID();
}

void D3D11_Pipeline::SetConstantBuffer(const uint& slot, const uint& scope, const D3D11_ConstantBuffer* buffer)
{
    if (buffer == nullptr || buffer->GetResource() == nullptr)
    {
        assert(false);
        return;
    }

    if (slot < 0 || slot >= D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT)
    {
        assert(false);
        return;
    }

    ID3D11Buffer* constant_buffers[]{ buffer->GetResource() };

    if (scope & ShaderScope_VS) device_context->VSSetConstantBuffers(slot, 1, constant_buffers);
    if (scope & ShaderScope_PS) device_context->PSSetConstantBuffers(slot, 1, constant_buffers);
}

void D3D11_Pipeline::SetShaderResource(const uint& slot, const uint& scope, const D3D11_Texture* resource)
{
    if (resource == nullptr || resource->GetResource() == nullptr)
    {
        assert(false);
        return;
    }

    if (slot < 0 || slot >= D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT)
    {
        assert(false);
        return;
    }

    ID3D11ShaderResourceView* shader_resources[]{ resource->GetResource() };

    if (scope & ShaderScope_VS) device_context->VSSetShaderResources(slot, 1, shader_resources);
    if (scope & ShaderScope_PS) device_context->PSSetShaderResources(slot, 1, shader_resources);
}

void D3D11_Pipeline::SetSamplerState(const uint& slot, const uint& scope, const D3D11_SamplerState* state)
{
    if (state == nullptr || state->GetResource() == nullptr)
    {
        assert(false);
        return;
    }

    if (slot < 0 || slot >= D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT)
    {
        assert(false);
        return;
    }

    ID3D11SamplerState* sampler_states[]{ state->GetResource() };

    if (scope & ShaderScope_VS) device_context->VSSetSamplers(slot, 1, sampler_states);
    if (scope & ShaderScope_PS) device_context->PSSetSamplers(slot, 1, sampler_states);
}

void D3D11_Pipeline::SetConstantBuffer_nullptr(const uint& slot, const uint& scope)
{
    if (slot < 0 || slot >= D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT)
    {
        assert(false);
        return;
    }

    ID3D11ShaderResourceView* constant_resources[]{ nullptr };

    if (scope & ShaderScope_VS) device_context->VSSetShaderResources(slot, 1, constant_resources);
    if (scope & ShaderScope_PS) device_context->PSSetShaderResources(slot, 1, constant_resources);
}

void D3D11_Pipeline::SetShaderResource_nullptr(const uint& slot, const uint& scope)
{
    if (slot < 0 || slot >= D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT)
    {
        assert(false);
        return;
    }

    ID3D11ShaderResourceView* sampler_resource[]{ nullptr };

    if (scope & ShaderScope_VS) device_context->VSSetShaderResources(slot, 1, sampler_resource);
    if (scope & ShaderScope_PS) device_context->PSSetShaderResources(slot, 1, sampler_resource);
}

void D3D11_Pipeline::Draw(const uint& vertex_count, const uint& vertex_offset)
{
    device_context->Draw(vertex_count, vertex_offset);
}

void D3D11_Pipeline::DrawIndexed(const uint& index_count, const uint& index_offset, const uint& vertex_offset)
{
    device_context->DrawIndexed(index_count, index_offset, vertex_offset);
}