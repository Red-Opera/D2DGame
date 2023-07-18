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

cbuffer AnimationBuffer : register(b2)
{
    float2 sprite_offset;
    float2 sprite_size;
    float2 texture_size;
    float isTexture;
}

PixelInput VS(VertexInput input)
{
	PixelInput output;
	
	// position.xy == (position.x, position.y), 가져온 픽셀 사이즈에 맞쳐서 화면에 출력
  //output.position = float4(input.position.xy * sprite_size, 0.0f, 1.0f); // 캐릭터의 크기를 가져온 스프라이트의 크기로 변경
  //output.position = mul(output.position, world);

	output.position = mul(input.position, world);
	output.position = mul(output.position, view);
	output.position = mul(output.position, projection);
	output.uv = input.uv;
	
    output.uv *= sprite_size / texture_size;
    output.uv += sprite_offset / texture_size;

	return output;
}

Texture2D source_texture : register(t0);
SamplerState samp : register(s0);

float4 PS(PixelInput input) : SV_Target
{
    if (isTexture == 1.0f)
		return source_texture.Sample(samp, input.uv);
	
	else
        return float4(1, 0, 0, 1);
}