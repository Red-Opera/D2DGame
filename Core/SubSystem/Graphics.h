// Graphics.h
// DX관련 데이터를 관리하고 백버퍼, ViewPort, Render Target View를 설정하는 파일

#pragma once

#include "Core/D3D11/Resterizer/D3D11_Viewport.h"
#include "ISubSystem.h"

class Graphics final : public ISubSystem
{
public:
	using ISubSystem::ISubSystem;
	~Graphics();

	// ISubSystem을(를) 통해 상속됨
	virtual bool Initialize() override;
	virtual void Update() override;
	
	void CreateBackBuffer(const uint& width, const uint& hegith);

	ID3D11Device* GetDevice() { return device; }							// device의 정보를 반환해주는 함수
	ID3D11DeviceContext* GetDeviceContext() { return device_context; }		// device_context의 정보를 반환해주는 함수

	void Begin();
	void End();

private:
	ID3D11Device* device = nullptr;								// DirectX 사용하기 위한 자원(버퍼, 텍스쳐) 생성
	ID3D11DeviceContext* device_context = nullptr;				// 핸들 변수(Device Context, DC), 자원(device)을 파이프 라인에 연결하는 변수
	IDXGISwapChain* swap_chain = nullptr;						// 더블 버퍼링, 백버퍼 처리 변수
	ID3D11RenderTargetView* render_target_view = nullptr;		// 그림을 그리는 타겟
	D3D11_Viewport viewport = { 0 };							// 그려질 Window창 화면 위치 및 영역을 저장하는 변수
	D3DXCOLOR clear_color = 0xff565656;							// 더블 버퍼에 사용할 때 화면을 초기화를 위한 색
};