// stdafx.h
#pragma once

#define NOMINMAX // numeric_limits<float>::max()에서 메크로를 제거하기 위한 define

#ifdef _DEBUG
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif // _DEBUG

#define DIRECTINPUT_VERSION 0x0800

#pragma warning(disable: 4506)

// STD
#include <Windows.h>
#include <assert.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <sstream>
#include <map>
#include <unordered_map>
#include <random>
#include <chrono>
#include <functional>
#include <memory>
#include <typeinfo>
#include <mutex>
#include <fstream>
#include <queue>

// DX
#include <d3dcompiler.h>
#include <d3d11.h>
#include <D3DX10math.h>
#include <D3DX11async.h>
#include <dinput.h>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx10.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

using uint = unsigned int;

// 매크로 함수
#define SAFE_RELEASE(p) {if(p) { (p)->Release(); (p) = nullptr; }}		// Com Interface용 해제
#define SAFE_DELETE(p) {if(p) { delete (p); (p) = nullptr; }}			// 동적할당용 해제
#define SAFE_DELETE_ARRAY(p) {if(p) { delete[] (p); (p) = nullptr; }}	// 배열용 해제

#define PI 3.141592
#define WorldX 2250.0f
#define WorldY 1125.0f

// Utility
#include "Utility/Geometry_Getnerator.h"
#include "Utility/GUID_Generator.h"
#include "Utility/Xml.h"
#include "Utility/ImGui/imgui.h"
#include "Utility/ImGui/imgui_impl_win32.h"
#include "Utility/ImGui/imgui_impl_dx11.h"
#include "Utility/LogSystem.h"
#include "Utility/TinyXML/tinystr.h"
#include "Utility/TinyXML/tinyxml.h"

// Math
#include "Utility/Math/Math.h"

// Intersect
#include "Intersect/Intersect.h"

// Framework
#include "Core/Settings.h"
#include "Core/Object.h"

// Main System
#include "Core/Context.h"

// Sub System
#include "Core/SubSystem/Graphics.h"
#include "Core/SubSystem/SceneManager.h"
#include "Core/SubSystem/Renderer/Renderer.h"
#include "Core/SubSystem/Timer.h"
#include "Core/SubSystem/ResourceManager.h"
#include "Core/SubSystem/InputManager.h"
#include "Core/SubSystem/ImGuiControl.h"
#include "Core/SubSystem/GetCursorSystem.h"
#include "Core/SubSystem/ChangeAttackTypeManager.h"
#include "Core/SubSystem/PressUI.h"

// D3D11
#include "Core/D3D11/Vertex/D3D11_Vertex.h"
#include "Core/D3D11/Vertex/D3D11_Geometry.h"

#include "Core/D3D11/InputAssembler/D3D11_VertexBuffer.h"
#include "Core/D3D11/InputAssembler/D3D11_IndexBuffer.h"
#include "Core/D3D11/InputAssembler/D3D11_InputLayout.h"

#include "Core/D3D11/Shader/D3D11_Shader.h"
#include "Core/D3D11/Shader/D3D11_ConstantBuffer.h"
#include "Core/D3D11/Shader/D3D11_Texture.h"
#include "Core/D3D11/Shader/D3D11_SamplerState.h"

#include "Core/D3D11/Resterizer/D3D11_ResterizerState.h"
#include "Core/D3D11/Resterizer/D3D11_Viewport.h"

#include "Core/D3D11/OutputManager/D3D11_Blend_state.h"

#include "Core/D3D11/D3D11_Pipeline.h"

// Resource
#include "Resource/Animation.h"
#include "Resource/StaticTexture.h"