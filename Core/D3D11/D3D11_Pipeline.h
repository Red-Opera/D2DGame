#pragma once

struct D3D11_PipelineState final
{
	D3D11_InputLayout* input_layout = nullptr;
	D3D11_PRIMITIVE_TOPOLOGY primitive_topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	D3D11_Shader* vertex_shader = nullptr;
	D3D11_Shader* pixel_shader = nullptr;
	D3D11_RasterizerState* rasterizer_state = nullptr;
	D3D11_Blend_state* blend_state = nullptr;
};

class D3D11_Pipeline final
{
public:
	D3D11_Pipeline(Graphics* graphics);
	~D3D11_Pipeline();

	bool Begin(const D3D11_PipelineState& pipeline_state);
	void End();

	void SetVertexBuffer(const D3D11_VertexBuffer* buffer);
	void SetIndexBuffer(const D3D11_IndexBuffer* buffer);

	void SetConstantBuffer(const uint& slot, const uint& scope, const D3D11_ConstantBuffer* buffer);
	void SetShaderResource(const uint& slot, const uint& scope, const D3D11_Texture* resource);
	void SetSamplerState(const uint& slot, const uint& scope, const D3D11_SamplerState* state);

	void SetConstantBuffer_nullptr(const uint& slot, const uint& scope);
	void SetShaderResource_nullptr(const uint& slot, const uint& scope);

	void Draw(const uint& vertex_count, const uint& vertex_offset = 0);
	void DrawIndexed(const uint& index_count, const uint& index_offset = 0, const uint& vertex_offset = 0);

private:
	ID3D11DeviceContext* device_context = nullptr;

	uint bind_vertex_buffer_id = 0;
	uint bind_index_buffer_id = 0;
	uint bind_inputlayout_id = 0;
	uint bind_primitive_topology_id = 0;
	uint bind_rasterizer_state_id = 0;
	uint bind_blend_state_id = 0;
};