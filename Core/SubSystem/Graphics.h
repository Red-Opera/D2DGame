// Graphics.h
// DX���� �����͸� �����ϰ� �����, ViewPort, Render Target View�� �����ϴ� ����

#pragma once

#include "Core/D3D11/Resterizer/D3D11_Viewport.h"
#include "ISubSystem.h"

class Graphics final : public ISubSystem
{
public:
	using ISubSystem::ISubSystem;
	~Graphics();

	// ISubSystem��(��) ���� ��ӵ�
	virtual bool Initialize() override;
	virtual void Update() override;
	
	void CreateBackBuffer(const uint& width, const uint& hegith);

	ID3D11Device* GetDevice() { return device; }							// device�� ������ ��ȯ���ִ� �Լ�
	ID3D11DeviceContext* GetDeviceContext() { return device_context; }		// device_context�� ������ ��ȯ���ִ� �Լ�

	void Begin();
	void End();

private:
	ID3D11Device* device = nullptr;								// DirectX ����ϱ� ���� �ڿ�(����, �ؽ���) ����
	ID3D11DeviceContext* device_context = nullptr;				// �ڵ� ����(Device Context, DC), �ڿ�(device)�� ������ ���ο� �����ϴ� ����
	IDXGISwapChain* swap_chain = nullptr;						// ���� ���۸�, ����� ó�� ����
	ID3D11RenderTargetView* render_target_view = nullptr;		// �׸��� �׸��� Ÿ��
	D3D11_Viewport viewport = { 0 };							// �׷��� Windowâ ȭ�� ��ġ �� ������ �����ϴ� ����
	D3DXCOLOR clear_color = 0xff565656;							// ���� ���ۿ� ����� �� ȭ���� �ʱ�ȭ�� ���� ��
};