// Window.h
// 윈도우 창을 띄우기 위한 헤더

#pragma once

#include "stdafx.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Window
{
    static HINSTANCE global_instance; // 인스턴스 정보 
    static HWND global_handle;        // 핸들의 정보

    /*
        - WndProc함수 : WinAPI의 처리 함수
            1. inline : 컴파일타임 때 정보를 불러온다. (빠르다, 무겁다)
            2. CALLBACK == __stdcall(callee -> API)
               cdecl(caller -> C)
            3. CALLBACK : 사용자가 호출하는 것이 아니라 메시지로 인하여 운영체제가 호출한다는 뜻(Call at the back) -> __stdcall과 의미상 다르기에 CALLBACK 으로 처리
    */

    inline LRESULT CALLBACK WndProc (HWND handle, UINT message, WPARAM wParam, LPARAM lParam )
    {
        if (ImGui_ImplWin32_WndProcHandler(handle, message, wParam, lParam))
            return true;

        // 메세지 처리
        switch (message)
        {
        case WM_CLOSE:   // 윈도우가 닫혔을 때
        case WM_DESTROY: // 윈도우가 파괴됐을 때
            PostQuitMessage(0); // WM_QUIT 메시지 발생 -> 종료 
            break;

        case WM_PAINT:
        {
            

            break;
        }
            
        default:
            // 위에서 원하는 내용을 처리하고, 남은 것을 처리해주는 기본 값
            return DefWindowProc(handle, message, wParam, lParam);
        }

        return 0;
    }

    // 윈도우 생성
    inline void Create(HINSTANCE hInstance, const UINT& width, const UINT& height)
    {
        // 윈도우를 만들기 위한 설명서 작성
        WNDCLASSEX wnd_class = { 0 };
        wnd_class.cbClsExtra = 0;
        wnd_class.cbWndExtra = 0;
        wnd_class.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
        wnd_class.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wnd_class.hIcon = LoadIcon(nullptr, IDI_ERROR);
        wnd_class.hIconSm = LoadIcon(nullptr, IDI_ERROR);
        wnd_class.hInstance = hInstance;
        wnd_class.lpfnWndProc = WndProc;
        wnd_class.lpszClassName = L"D2D11Game";
        wnd_class.lpszMenuName = nullptr;
        wnd_class.style = CS_HREDRAW | CS_VREDRAW;
        wnd_class.cbSize = sizeof(WNDCLASSEX);

        // 클래스 등록
        auto check = RegisterClassEx(&wnd_class);
        assert(check != 0); // 등록 확인

        // 윈도우 생성
        global_handle = CreateWindowExW
        (
            WS_EX_APPWINDOW,
            L"D2D11Game",
            L"D2D11Game",
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            static_cast<int>(width),
            static_cast<int>(height),
            nullptr,
            nullptr,
            hInstance,
            nullptr
        );

        global_instance = hInstance;

        // 생성 체크 
        assert(global_handle != nullptr);
    }

    // 윈도우 출력
    inline void Show()
    {
        SetForegroundWindow(global_handle);
        SetFocus(global_handle);
        ShowCursor(TRUE);
        ShowWindow(global_handle, SW_NORMAL);
        UpdateWindow(global_handle);
    }

    // 메시지 루프
    inline const bool Update()
    {
        MSG msg;
        ZeroMemory(&msg, sizeof(MSG));

        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // WM_QUIT 메시지를 반환하기 전까지 계속 반복하라는 의미로 true 값 반환
        return msg.message != WM_QUIT;
    }

    // 윈도우 파괴
    inline void Destroy()
    {
        DestroyWindow(global_handle);
        UnregisterClass(L"D2D11Game", global_instance);
    }

    // 윈도우 가로 구하기
    inline const UINT GetWidth()
    {
        RECT rect;
        GetClientRect(global_handle, &rect);                // 사용자의 화면 크기 가져옴

        return static_cast<UINT>(rect.right - rect.left);   // Right값에서 Left값을 뺴서 화면의 너비를 반환함
    }

    // 윈도우 세로 구하기
    inline const UINT GetHeight()
    {
        RECT rect;
        GetClientRect(global_handle, &rect);

        return static_cast<UINT>(rect.bottom - rect.top);   // Bottom값에서 top값을 빼서 높이를 반환함
    }
}