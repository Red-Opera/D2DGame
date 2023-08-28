// Window.h
// ������ â�� ���� ���� ���

#pragma once

#include "stdafx.h"
#include "resource.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Window
{
    static HINSTANCE global_instance; // �ν��Ͻ� ���� 
    static HWND global_handle;        // �ڵ��� ����

    inline LRESULT CALLBACK WndProc (HWND handle, UINT message, WPARAM wParam, LPARAM lParam )
    {
       //if (ImGui_ImplWin32_WndProcHandler(handle, message, wParam, lParam))
        //  return true;

        // �޼��� ó��
        switch (message)
        {
        case WM_CLOSE:   // �����찡 ������ ��
        case WM_DESTROY: // �����찡 �ı����� ��
            PostQuitMessage(0); // WM_QUIT �޽��� �߻� -> ���� 
            break;

        case WM_PAINT:
        {
            

            break;
        }
            
        default:
            // ������ ���ϴ� ������ ó���ϰ�, ���� ���� ó�����ִ� �⺻ ��
            return DefWindowProc(handle, message, wParam, lParam);
        }

        return 0;
    }

    // ������ ����
    inline void Create(HINSTANCE hInstance, const UINT& width, const UINT& height)
    {
        // �����츦 ����� ���� ���� �ۼ�
        WNDCLASSEX wnd_class = { 0 };
        wnd_class.cbClsExtra = 0;
        wnd_class.cbWndExtra = 0;
        wnd_class.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
        wnd_class.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wnd_class.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
        wnd_class.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
        wnd_class.hInstance = hInstance;
        wnd_class.lpfnWndProc = WndProc;
        wnd_class.lpszClassName = L"D2D11Game";
        wnd_class.lpszMenuName = nullptr;
        wnd_class.style = CS_HREDRAW | CS_VREDRAW;
        wnd_class.cbSize = sizeof(WNDCLASSEX);

        // Ŭ���� ���
        auto check = RegisterClassEx(&wnd_class);
        assert(check != 0); // ��� Ȯ��

        // ������ ����
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
        MoveWindow(global_handle, 0, 0, width, height, false);

        // ���� üũ 
        assert(global_handle != nullptr);
    }

    // ������ ���
    inline void Show()
    {
        SetForegroundWindow(global_handle);
        SetFocus(global_handle);
        ShowCursor(TRUE);
        ShowWindow(global_handle, SW_NORMAL);
        UpdateWindow(global_handle);
    }

    // �޽��� ����
    inline const bool Update()
    {
        MSG msg;
        ZeroMemory(&msg, sizeof(MSG));

        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // WM_QUIT �޽����� ��ȯ�ϱ� ������ ��� �ݺ��϶�� �ǹ̷� true �� ��ȯ
        return msg.message != WM_QUIT;
    }

    // ������ �ı�
    inline void Destroy()
    {
        DestroyWindow(global_handle);
        UnregisterClass(L"D2D11Game", global_instance);
    }

    // ������ ���� ���ϱ�
    inline const UINT GetWidth()
    {
        RECT rect;
        GetClientRect(global_handle, &rect);                // ������� ȭ�� ũ�� ������

        return static_cast<UINT>(rect.right - rect.left);   // Right������ Left���� ���� ȭ���� �ʺ� ��ȯ��
    }

    // ������ ���� ���ϱ�
    inline const UINT GetHeight()
    {
        RECT rect;
        GetClientRect(global_handle, &rect);

        return static_cast<UINT>(rect.bottom - rect.top);   // Bottom������ top���� ���� ���̸� ��ȯ��
    }
}