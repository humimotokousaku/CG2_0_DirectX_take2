#include "ImGuiManager.h"

void ImGuiManager::Initialize(ID3D12Device* device, DXGI_SWAP_CHAIN_DESC1 swapChainDesc, D3D12_RENDER_TARGET_VIEW_DESC rtvDesc,ID3D12DescriptorHeap* srvDescriptorHeap) {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(WinApp::hwnd_);
	ImGui_ImplDX12_Init(device,
		swapChainDesc.BufferCount,
		rtvDesc.Format,
		srvDescriptorHeap,
		srvDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
		srvDescriptorHeap->GetGPUDescriptorHandleForHeapStart());
}

void ImGuiManager::PreDraw() {
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::ShowDemoWindow();
}

void ImGuiManager::PostDraw(ID3D12GraphicsCommandList* commandList) {
	ImGui::Render();
	// 実際のcommandListのImGuiの描画コマンドを積む
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList);
}

void ImGuiManager::Release() {
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext(); // ImGuiのコンテキストを破棄
}