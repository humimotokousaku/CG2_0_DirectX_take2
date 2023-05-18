#pragma once
#include <stdint.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxgidebug.h>

class DirectXCommon
{
public:

	void DebugLayer();

	void Initialize();

	void WindowChangeColor();

	// 解放処理とリソースチェックが入ってる
	void Release();


public:
	static ID3D12Debug1* debugController_;
	static UINT backBufferIndex_;
	static IDXGIFactory7* dxgiFactory_;
	static HRESULT hr_;
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
};

