#pragma once
#include <stdint.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxgidebug.h>
#include "imguiManager.h"
#include "TextureManager.h"

class DirectXCommon
{
public:
	// Getter
	ID3D12Device* GetDevice() { return this->device_; }
	ID3D12GraphicsCommandList* GetCommandList() { return this->commandList_; }
	ID3D12DescriptorHeap* GetSrvDescriptorHeap() { return this->srvDescriptorHeap_; }
	D3D12_RENDER_TARGET_VIEW_DESC GetRtvDesc() { return rtvDesc_; }
	DXGI_SWAP_CHAIN_DESC1 GetSwapChainDesc() { return swapChainDesc_; }

	// 使用するアダプタを決定する
	void GetAdapter();

	// D3D12Deviceの生成
	void CreateD3D12Device();

	// エラーや警告が起きると止まる
	void StopError();

	// コマンドキューの生成
	void CreateComandQueue();

	// コマンドリストの生成
	void CreateComandList();

	// SwapChainの生成
	void CreateSwapChain();

	// DescriptorHeapの生成
	ID3D12DescriptorHeap* CreateDescriptorHeap(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible);

	// 二つのswapChainResoucesを取得
	void GetSwapChainResources();

	// RTVを作る
	void CreateRTV();

	// メインループ前の初期化
	void Initialize();

	// 描画前の処理
	void PreDraw(ID3D12DescriptorHeap* dsvDescriptorHeap);

	// 描画後の処理
	void PostDraw();

	// 解放処理とリソースチェック
	void Release();

private:
	static UINT backBufferIndex_;
	static IDXGIFactory7* dxgiFactory_;
	static IDXGIAdapter4* useAdapter_;
	static ID3D12Device* device_;
	static ID3D12InfoQueue* infoQueue_;
	static ID3D12CommandQueue* commandQueue_;
	static ID3D12CommandAllocator* commandAllocator_;
	static ID3D12GraphicsCommandList* commandList_;
	static IDXGISwapChain4* swapChain_;
	static DXGI_SWAP_CHAIN_DESC1 swapChainDesc_;
	static D3D12_RENDER_TARGET_VIEW_DESC rtvDesc_;
	static ID3D12DescriptorHeap* rtvDescriptorHeap_;
	static ID3D12DescriptorHeap* srvDescriptorHeap_;
	static ID3D12Resource* swapChainResources_[2];
	static D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles_[2];
	static ID3D12Fence* fence_;
	static uint64_t fenceValue_;
	static HANDLE fenceEvent_;
	static D3D12_RESOURCE_BARRIER barrier_;

	ImGuiManager imGuiManager_;
};

