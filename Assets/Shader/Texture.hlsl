// Texture.hlsl

struct VertexInput
{
	float4 position : POSITION0;
	float2 uv : TEXCOORD0;
};

struct PixelInput
{
	float4 position : SV_POSITION0;
	float2 uv : TEXCOORD0;
};

cbuffer CameraBuffer : register(b0)
{
	matrix view;
	matrix projection;
}

cbuffer TransformBuffer : register(b1)
{
    matrix world;
}

PixelInput VS(VertexInput input)
{

	return input;
}

Texture2D source_texture1 : register(t0); // t0 : texture의 약자
Texture2D source_texture2 : register(t1);
SamplerState samp : register(s0);

float4 PS(PixelInput input) : SV_Target
{
    float4 color = source_texture1.Sample(samp, input.uv);
	// clip() : 인자로 받은 데이터가 0보다 작다면 해당 픽셀을 폐기한다.
	// clip(color.a - 0.1f);

	//if (color.a < 0.1f)
	//	discard; // 호출되는 순간 픽셀을 폐기시키는 키워드

	// 텍스처의 색깔을 추가
	//color.r += 0.2f;
	//color.b += 0.2f;
	//color.g += 0.2f;

	return color;
}