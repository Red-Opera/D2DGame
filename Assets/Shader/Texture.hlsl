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

Texture2D source_texture1 : register(t0); // t0 : texture�� ����
Texture2D source_texture2 : register(t1);
SamplerState samp : register(s0);

float4 PS(PixelInput input) : SV_Target
{
    float4 color = source_texture1.Sample(samp, input.uv);
	// clip() : ���ڷ� ���� �����Ͱ� 0���� �۴ٸ� �ش� �ȼ��� ����Ѵ�.
	// clip(color.a - 0.1f);

	//if (color.a < 0.1f)
	//	discard; // ȣ��Ǵ� ���� �ȼ��� ����Ű�� Ű����

	// �ؽ�ó�� ������ �߰�
	//color.r += 0.2f;
	//color.b += 0.2f;
	//color.g += 0.2f;

	return color;
}