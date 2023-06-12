#pragma once
#include <stdint.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxgidebug.h>

class DirectXCommon
{
public:
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
	void CreateDescriptorHeap();

	// RTVを作る
	void CreateRTV();

	// メインループ前の初期化
	void Initialize();

	// 描画前の処理
	void PreDraw();

	// 描画後の処理
	void PostDraw();

	// getter
	ID3D12Device* GetDevice() { return this->device_; }
	ID3D12GraphicsCommandList* GetCommandList() { return this->commandList_; }

	// 解放処理とリソースチェック
	void Release();

public:
	static UINT backBufferIndex_;
	static IDXGIFactory7* dxgiFactory_;
	static IDXGIAdapter4* useAdapter_;
	static ID3D12Device* device_;
	static ID3D12InfoQueue* infoQueue_;
	static ID3D12CommandQueue* commandQueue_;
	static ID3D12CommandAllocator* commandAllocator_;
	static ID3D12GraphicsCommandList* commandList_;
	static IDXGISwapChain4* swapChain_;
	static ID3D12DescriptorHeap* rtvDescriptorHeap_;
	static ID3D12Resource* swapChainResources_[2];
	static D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles_[2];
	static ID3D12Fence* fence_;
	static uint64_t fenceValue_;
	static HANDLE fenceEvent_;
	static IDXGIDebug1* debug_;
	static D3D12_RESOURCE_BARRIER barrier_;
};

