#pragma once
#include <stdint.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxgidebug.h>

class DirectXCommon
{
public:
	static DirectXCommon* GetInstance();

	~DirectXCommon();

	// getter
	ID3D12Device* GetDevice() { return this->device_; }
	ID3D12GraphicsCommandList* GetCommandList() { return this->commandList_; }

	// メインループ前の初期化
	void Initialize();

	// 描画前の処理
	void PreDraw();

	// 描画後の処理
	void PostDraw();

	// 解放処理とリソースチェック
	void Finalize();

private:
	UINT backBufferIndex_;
	IDXGIFactory7* dxgiFactory_;
	IDXGIAdapter4* useAdapter_;
	ID3D12Device* device_;
	ID3D12InfoQueue* infoQueue_;
	ID3D12CommandQueue* commandQueue_;
	ID3D12CommandAllocator* commandAllocator_;
	ID3D12GraphicsCommandList* commandList_;
	IDXGISwapChain4* swapChain_;
	ID3D12DescriptorHeap* rtvDescriptorHeap_;
	ID3D12Resource* swapChainResources_[2];
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles_[2];
	ID3D12Fence* fence_;
	uint64_t fenceValue_;
	HANDLE fenceEvent_;
	D3D12_RESOURCE_BARRIER barrier_;
};

