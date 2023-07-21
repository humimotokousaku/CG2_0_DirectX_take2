#include "ImGuiManager.h"

void ImGuiManager::Initialize(const Microsoft::WRL::ComPtr<ID3D12Device>& device, DXGI_SWAP_CHAIN_DESC1 swapChainDesc, D3D12_RENDER_TARGET_VIEW_DESC rtvDesc, const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& srvDescriptorHeap, HWND hwnd) {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX12_Init(device.Get(),
		swapChainDesc.BufferCount,
		rtvDesc.Format,
		srvDescriptorHeap.Get(),
		srvDescriptorHeap.Get()->GetCPUDescriptorHandleForHeapStart(),
		srvDescriptorHeap.Get()->GetGPUDescriptorHandleForHeapStart());
}

void ImGuiManager::PreDraw() {
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::ShowDemoWindow();
}

void ImGuiManager::PostDraw(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList) {
	ImGui::Render();
	// 実際のcommandListのImGuiの描画コマンドを積む
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList.Get());
}

void ImGuiManager::Release() {
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext(); // ImGuiのコンテキストを破棄
}