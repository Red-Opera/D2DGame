// Color.hlsl
// ������ ���������� ���� ����

// IA���� ���� ������ ��� ����ü
struct VertexInput
{
	/*
		POSITION0 -> Sementic
		
		Sementic
			- �������� ��ó�� ���ҿ� ���� �ǹ̸� �ο��ϱ� ���� Ű���� (�ش� Ű����� ����ڰ� �����ϴ� ���� �ƴ϶� �̹� ������ ����)
			- POSITON0���� POSITION�� �̸�, 0�� �ε����̴�.
	*/

	float4 position : POSITION0;	// float4 == float[4]
	float4 color : COLOR0;
};

// Pixel Shader�� PS�ܰ�� ������ ���� ����ü
struct PixelInput
{
	/*
		System Value(SV)
			- �ý��ۿ��� ����� ���̶�� ����� ��
			- SV_ ���λ縦 ���� �ش� ���� �ý��ۿ��� Ư���� �ǹ̰� �ִٰ� ǥ���� ���̴�.
			- SV�� ������ ������ ������ �ܰ迡 ���� �ǹ̰� �ٸ� �� �ִ�.
			- �ش� ���λ�� �ý����� ����� ���� �������ְų�, �ý��ۿ��� ��� �ǹ̸� ���� ���� ���� �� �ִ�.
			- SV_�� �ٴ� �پ��� Ű���尡 �ְ� �̰� �������� Ȯ��
			  (��ũ : https://docs.microsoft.com/ko-kr/windows/win32/direct3dhlsl/dx-graphics-hlsl-semantics?redirectedfrom=MSDN)
			- VS�ܰ迡���� SV_POSITION�� ���� �ش� SV�� ���� �����͸� ÷���Ͽ� ����� ���(PixelInput VS�Լ����� return output),
			  �ش� ���� RS�ܰ迡�� ������ �̹���ȭ�� �� �� �����, VS�ܰ迡�� ���� ��ȯ������ ���ļ� ���� �����̶�� �ǹ̸� �ý��ۿ��� ������ �� �ִ�.
			- PS�ܰ迡���� SV_POSITION�� ���� VS�ܰ迡�� �ѱ� SV_POSTION�� ���� PS �ܰ谡 ���� ����
			  RS�ܰ踦 ��ġ�鼭 �ϼ��� ������ �̹����� �ȼ� ��ǥ���� �ްڴٴ� �ǹ��̴�.

		������ �̹��� : �ȼ��� �̹����� ǥ���ϴ� �ڷᱸ��, ��Ʈ���̶�� �θ���.
	*/

	float4 position : SV_POSITION0;	// RS�ܰ迡�� �ȼ�ȭ �� �� ����϶�� ��
	float4 color : COLOR0;
};

/*
	- cbuffer : Constant Buffer�� ����
	- register : �����Ϳ� �����͸� ���� �����ش�.
		1. b : buffer
		2. 0 : slot, ������ �������� ������ ��ȣ, 0 ~ 13 ����(b0 ~ b13)�� ����
	- matrix : 4x4 ���


	��� ���۴� 16Byte ������ �����ؾ� �Ѵ�.
	�ִ� 4096 * 16Byte��ŭ ���� �� �ִ�.
*/

cbuffer CameraBuffer : register(b0)
{
    matrix view;
    matrix projection;
}

cbuffer TransformBuffer : register(b1)
{
    matrix world;
}

cbuffer ColorBuffer : register(b2)
{
    float4 color;
}

// VS �ܰ� : IA���� ���� ������ ������ȯ���ִ� �ܰ�
PixelInput VS(VertexInput input)
{
	PixelInput output;

	/*
		1 x 4(float4 position) * 4 x 4(world) = 1 x 4
 	*/

	// ���� ������ ���� SV_���λ縦 �ٿ��ش�. -> RS�ܰ迡�� ����� ������ �������� ���̴�.
	// output.position = input.position;

	// ��� ���� ���� �����̵��� �����ش�.
	output.position = mul(input.position, world);		// world�������� ��ȯ�� ���� �����Ϳ� ����
	output.position = mul(output.position, view);		// view�������� ��ȯ�� ���� �����Ϳ� ����
	output.position = mul(output.position, projection); // projection�������� ��ȯ�� ���� �����Ϳ� ���� (Direct X������ Clip������ ���ÿ� ����)
	output.color = input.color;

	return output;	// ��ȯ�� ���� RS�ܰ踦 ��ġ�� �ȴ�.
}

// SV_Target : ���� ���õ� ���� ���� Ÿ���� ����ض�
float4 PS(PixelInput input) : SV_Target // �ϼ��� ������ ���� �����ϴ��� �˷����� ������ ���ܰ� �߻��Ѵ�.
{
    return input.color;
}