// Graphics.cpp

#include "stdafx.h"
#include "Graphics.h"

Graphics::~Graphics()
{
	// ���α׷��� ����� �� �������� ��� �ڿ��� ������, ������ �������� ������
	SAFE_RELEASE(render_target_view);
	SAFE_RELEASE(device);
	SAFE_RELEASE(device_context);
	SAFE_RELEASE(swap_chain);
}

// �׷��� �۾��� �� �� ���� ���� ó���ϴ� �Լ�
bool Graphics::Initialize()
{
	DXGI_SWAP_CHAIN_DESC desc;							// DXGI_SWAP_CHAIN_DESC : ���� ü���� ����� ���� ����
	ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC));

	// ����ü���� ������ ������ ũ�� (������� ũ��)
	desc.BufferDesc.Width = 0;							// �ʺ�
	desc.BufferDesc.Height = 0;							// ����

	// RefreshRate : ȭ�� �ֻ���, ȭ���� �����ֱ�, �ʴ� "Numerator/Denominator"��ŭ ���� (��ȯ�� : unsigned int -> ���� ������)
	desc.BufferDesc.RefreshRate.Numerator = 60;			// ����
	desc.BufferDesc.RefreshRate.Denominator = 1;		// �и�	

	/*
		- RGBA
			1. ���� ä���� �ǹ�
			2. ������ ���� �����ͷ� ���� �ʿ�� ����. -> ��, ������ �ƴ� ��ǥ�� ������ ��� ������ �������ε� ����� �� �ִ�. (ex : R32G32 -> int�� ��ǥ��)
			3. R8G8B8A8 : BPC, ColorDepth��� �θ��� ���� ä���� ���� �޸𸮸� �Ҵ����� �ǹ��ϱ⵵ �Ѵ�.
			   R32G32 : 4����Ʈ�� ������ �ڷḦ 2���� �����ڴٴ� ��

		- UNORM(Unsigned Normalize) : ����ȭ
			1. 0 ~ 1�� ����ȭ�� �����͸� ���
			2. �� ä���� 8��Ʈ�� ���¿��� ����ȭ�� �� ��� 0 ~ 255 ������ 0 ~ 1�� ���·� �ٲ۴ٴ� ��

		DXGI_FORMAT_R8G8B8A8_UNORM -> 4����Ʈ ũ��(�� ä�δ� 8��Ʈ * 4)�� ������ ����ȭ�� �����͸� ��ڴ�.
	*/

	// ������ ���÷��� ���� ����
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // RGBA������ 8��Ʈ(1����Ʈ)�� �Ҵ��ϰڴٴ� �� -> �� ä���� 8��Ʈ�̴� ä�κ��� 0 ~ 255�� ������ ������.

	/*
		ScanlineOrdering
		
		- ȭ���� ����� �� �ȼ��� � ������� �� �پ� ����ϴ��� �����ϴ� ����
		- Scanline : �� �پ� ���
		- ��� 
			1. UNSPECIFIED : �������� ����
			2. PROGRESSIVE : ������ �Ʒ��� ���ʴ�� ���
			3. UPPER_FIELD_FIRST : Ȧ����° �� ���� ����ϰ� ¦����° �� ���
			4. LOWER_FIELD_FIRST : ¦����° �� ���� ����ϰ� Ȧ����° �� ���
	*/

	desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; // �Ϲ������� ȭ�� ���

	// ȭ���� ���/Ȯ�� �� �� ������� ȭ�� ���� ���� ó�����
	desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	desc.BufferCount = 1;								// ������ ����
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// ������ �뵵 �Ǵ� ��� (DXGI_USAGE_RENDER_TARGET_OUTPUT : ��� ������ �� ���, ����۸� �׸��� �뵵)

	/*
		- Aliasing : �ȼ� ǥ������ ���� Ȯ���ϸ� �ܰ����� ���ó�� ���̴� ����
		- Anti Aliasing(AA) - ��� ������ �����ϴ� ���

			1. Super Sampling(SSAA)
			- ���ø� �����ŭ �ش絵�� Ȯ���� �Ŀ� Down Sampling(�ػ󵵸� �ٿ� ���� ���´�)�� �ϴ� ���
			- ���� ���귮�� �޸𸮰� �ʿ��ϴ�.

			2. Multi Sampling AA(MSAA : ���� ǥ��ȭ ��Ƽ�������)
			- ���� ���귮�� �޸𸮸� �䱸�ϴ� SSAA�� �����ؼ� ���� ���
			- ������(�̹���)�� �����ڸ��� AA�� �����Ѵ�.

		// ���� : ��Ƽ �ٸ���� �����ϱ�
		// ���� : BPC ����
	*/

	desc.SampleDesc.Count = 1;					// ��Ƽ��������� ���� ����, ���(�󸶸�ŭ �ø� ���ΰ�)
	desc.SampleDesc.Quality = 0;				// ���ø��� ����Ƽ ����, 0�� �Ⱦ��ڴٴ� �ǹ�

	// ������ ���� �ɼ�
	desc.OutputWindow = Settings::Get().GetWindowHandle();	// ����� ������ �ڵ� (�Է°� : Window �ڵ�)
	desc.Windowed = TRUE;									// â��� ���� (FALSE�� �� ��� �Ϲ����� ����ó�� ȭ���� ��üȭ���� �ǰ� �ִ���� �� ���� �޴��� ��������.)

	// ����ۿ� ���ҽ� � ȿ���� �� ���ΰ�?
	desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; // DISCARD : �����Ͽ� �ٸ� �����͸� ����Ѵ�.

	// FEATURE_LEVEL : ��� ����, DX ��� ������ �� ���ΰ�?
	std::vector<D3D_FEATURE_LEVEL> feature_levels
	{
	   D3D_FEATURE_LEVEL_11_1,
	   D3D_FEATURE_LEVEL_11_0,
	   D3D_FEATURE_LEVEL_10_1,
	   D3D_FEATURE_LEVEL_10_0,
	};

	/*
		D3D11CreateDeviceAndSwapChain : ����̽��� ����ü���� ������ִ� �Լ�
			- ��ȯ�� : HRESULT -> �Լ� ����� �������θ� ��� �ڷ��� (S_OK(����) �Ǵ� S_FALSE(����)�� ���� ������ ��������)
	*/

	HRESULT hr = D3D11CreateDeviceAndSwapChain
	(
		nullptr,					// ���� ī��(�׷��� ī��)�� �����ؼ� ����� �� ����
		D3D_DRIVER_TYPE_HARDWARE,	// ����̽� Ÿ�� (D3D_DRIVER_TYPE_SOFRWARE : ������ �� �� ����Ʈ����� �� ��� -> �׽�Ʈ �� �������� �� �ϵ������ ������ ���� �ʴ� �پ��� ȯ�濡�� ����� �� ���)
		nullptr,					// �ٸ� ����Ʈ��� �߰������� ���� ���ΰ�?
		0,							// ����̽��� ���� �� � ����� �߰������� ���� ���ΰ�?
		feature_levels.data(),		// ����ü���� ����� ����, ��ó ����(FEATURE_LEVEL) �迭�� ���� �ּ�
		feature_levels.size(),		// ��ó ���� ī��Ʈ(�迭�� ���)
		D3D11_SDK_VERSION,			// ����ϴ� SDK ����
		&desc,						// ����ü���� ���� �� ����ϴ� desc �ּ�
		&swap_chain,				// ������ ����ü���� �Ҵ���� ������ ������ �ּ�
		&device,					// ������ ����̽��� �Ҵ���� ������ ������ ����
		nullptr,					// ���õ� ��ó������ ��ȯ���� ������ �ּ� (���õ� DX������ ��ȯ��)
		&device_context				// ������ ����̽� ���ý�Ʈ�� �Ҵ���� ������ ������ �ּ�
	);

	assert(SUCCEEDED(hr));			// SUCCEEDED() : HRESULT���� ������� ������ true, false�� ��ȯ���ִ� ��ũ��

	return true;
}

void Graphics::Update()
{
	
}

void Graphics::CreateBackBuffer(const uint& width, const uint& height)
{
	// ===============================================================
	//						����� ���� ����
	// ===============================================================
	/*
		ResizeBuffers
			- ���� ü���� ������ ���� ������ �����ϴ� �Լ�
			- ������ ���� �־��ְ� �������� ���� ���� �ʱⰪ�� ������ ������ ���� �ʴ´�.
	*/

	// ����ü�ο� �ִ� BufferDesc.Width�� BufferDesc.Height�� �����ϱ� ���ؼ� swap_chain�� ResizeBuffer�Լ��� ȣ����
	HRESULT hr = swap_chain->ResizeBuffers
	(
		0,						// ������ ����	(����X)
		width,					// �ʺ�
		height,					// ����
		DXGI_FORMAT_UNKNOWN,	// ������ ����	(����X)
		0						// �÷���		(����X)
	);
	assert(SUCCEEDED(hr));

	// ===============================================================
	//					RTV(render_target_view) ����
	// ===============================================================
	/*
		Interface Idenrifier(IID)
			- �������̽��� �����ϱ� ���� �ĺ���
			- GUID�� �� �� �ϳ�

		Globally Unique Identifier(GUID)
			- MS���� ����ϴ� ���� �׸��� �����ϱ� ���� �ĺ���
			- 16����Ʈ�� ũ�⸦ ������. (������ �ؼ� ���� �̴´�.)
			- U(Universal)UID��� �ִµ� ū ���̴� ����.

		���� : GUID, UUID ����
	*/

	// ����ü�ο� �ִ� ���۸� �������� �κ�
	ID3D11Texture2D* back_buffer = nullptr;		// ���۸� 2D������ ������ ������

	hr = swap_chain->GetBuffer
	(
		0,										// ����ü���� ���°�� ���۸� ���� ���ΰ�?
		__uuidof(ID3D11Texture2D),				// ����۰� ���� ��ȯ�� UUID��, __uuidof : �ڷ������� �����ϰ� �������� UUID���� �������� Ű����
		reinterpret_cast<void**>(&back_buffer)	// ��ȯ�� �������̽��� ���� �������, void** : Texture 3D�Ǵ� 1D�� ���� �ֱ� ����
	);
	assert(SUCCEEDED(hr));

	// render_target_view�� ����� �κ�
	hr = device->CreateRenderTargetView			// buffer�� ����� ������ ������� render_target_view�� ���� ������
	(
		back_buffer,							// � �ڿ��� �̟G�ؼ� �信�� ������ ���ҽ�
		nullptr,								// RTV�� desc, nullptr�� ��� ���ҽ��� ������ ������.
		&render_target_view						// RTV�� �޾ƿ� ���� -> �ش� ��ġ���� back_buffer�� ���� �Ѱ������Ƿ� back_buffer�� �������൵ �ȴ�.
	);
	assert(SUCCEEDED(hr));


	// ===============================================================
	//			����Ʈ(�׸��� �׸� ����) ���� -> WIndowâ ũ�⸦ ����
	// ===============================================================
	viewport.x = 0.0f;						// ���� ������ ���� x��ǥ
	viewport.y = 0.0f;						// ���� ������ ���� y��ǥ
	viewport.width = static_cast<float>(width);		// Windowâ�� �ʺ�
	viewport.height = static_cast<float>(height);	// Windowâ�� ����
	viewport.min_depth = 0.0f;						// 3D���� �ּ� ����
	viewport.max_depth = 1.0f;						// 3D���� �ִ� ���� (ũ�⸦ 0���� �ϸ� ���°��� �Ǳ� ������ 2D���� �ִ�� �ּ��� ���̴� ������ �ȵȴ�.)

	// ����
	SAFE_RELEASE(back_buffer);
}

// Graphics�� Begin�Լ��� ������ Execute�� �߰��ܰ迡 �ۼ��ص� ������ ���� �ۼ�
void Graphics::Begin()
{
	/*
		 Output Merger Stage(OM) - ��� ���� �ܰ�
		 - �׸��� �׸� ���� Ÿ���� �����ϰ�, ���� ��� ���� ���ΰ�(����)�� �������ش�.
		 - OM�� ���� �� �� �ִ� ���� Ÿ���� ������ 8���̴�. -> 8�� ���ÿ� �׸� �� �ִ�.
	*/

	device_context->OMSetRenderTargets(1, &render_target_view, nullptr);	// �׸��� �׸� ����Ÿ���� ����

	D3D11_VIEWPORT d3d11_viewport;

	d3d11_viewport.TopLeftX = viewport.x;
	d3d11_viewport.TopLeftY = viewport.y;
	d3d11_viewport.Width = viewport.width;
	d3d11_viewport.Height = viewport.height;
	d3d11_viewport.MinDepth = viewport.min_depth;
	d3d11_viewport.MaxDepth = viewport.max_depth;

	device_context->RSSetViewports(1, &d3d11_viewport);						// �׸� ���� ����
	device_context->ClearRenderTargetView(render_target_view, clear_color);	// ���� Ÿ��(��ȭ��) �ʱ�ȭ
}

void Graphics::End()
{
	// ���� ���۸��� �����ϴ� swap_chain�� �ִ� Present�� �̿��Ͽ� ȭ�� �������� ����
	HRESULT hr = swap_chain->Present(1, 0); // ���� ����ȭ ��, �÷��� ��
	assert(SUCCEEDED(hr));
}