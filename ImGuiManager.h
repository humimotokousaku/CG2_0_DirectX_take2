#pragma once
#include "WinApp.h"
#include <stdint.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxgidebug.h>

#include "externals/ImGui/imgui.h"
#include "externals/ImGui/imgui_impl_dx12.h"
#include "externals/ImGui/imgui_impl_win32.h"
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

class ImGuiManager
{
public:
	void Initialize(ID3D12Device* device, DXGI_SWAP_CHAIN_DESC1 swapChainDesc, D3D12_RENDER_TARGET_VIEW_DESC rtvDesc, ID3D12DescriptorHeap* srvDescriptorHeap);
	void PreDraw();
	void PostDraw(ID3D12GraphicsCommandList* commandList);
	void Release();
};

