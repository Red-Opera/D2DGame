#include "stdafx.h"
#include "D3D11_Blend_state.h"

D3D11_Blend_state::D3D11_Blend_state(Graphics* graphics)
{
	device = graphics->GetDevice();
}

D3D11_Blend_state::~D3D11_Blend_state()
{
	Clear();
}

void D3D11_Blend_state::Create(const bool& is_blend_enabled, const D3D11_BLEND& src_blend, const D3D11_BLEND& dst_blend, const D3D11_BLEND_OP& blend_op, const D3D11_BLEND& src_blend_alpha, const D3D11_BLEND& dst_blend_alpha, const D3D11_BLEND_OP& blend_op_alpha, const float& factor, const D3D11_COLOR_WRITE_ENABLE& color_mask)
{
	D3D11_BLEND_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BLEND_DESC));

	desc.AlphaToCoverageEnable = false;
	desc.IndependentBlendEnable = false;
	desc.RenderTarget[0].BlendEnable = is_blend_enabled;
	desc.RenderTarget[0].SrcBlend = src_blend;
	desc.RenderTarget[0].DestBlend = dst_blend;
	desc.RenderTarget[0].BlendOp = blend_op;
	desc.RenderTarget[0].SrcBlendAlpha = src_blend_alpha;
	desc.RenderTarget[0].DestBlendAlpha = dst_blend_alpha;
	desc.RenderTarget[0].BlendOpAlpha = blend_op_alpha;
	desc.RenderTarget[0].RenderTargetWriteMask = color_mask;
	blend_factor = factor;

	HRESULT hr = device->CreateBlendState(&desc, &state);
	assert(SUCCEEDED(hr));
}

void D3D11_Blend_state::Clear()
{
	SAFE_RELEASE(state);
	blend_factor = 0.0f;
	sample_mask = 0xffffffff;
}
