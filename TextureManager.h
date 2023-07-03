#pragma once
#include "externals/DirectXTex/d3dx12.h"
#include "externals/DirectXTex/DirectXTex.h"

#include <string>
#include "Triangle.h"

class TextureManager
{
public:
	// Getter
	D3D12_GPU_DESCRIPTOR_HANDLE GetTextureSrvHandleGPU() { return textureSrvHandleGPU_; }

	// 中間リソースの生成
	ID3D12Resource* CreateBufferResource(ID3D12Device* device, size_t sizeInBytes);

	// COMの初期化
	void ComInit();

	// textureデータを読む
	DirectX::ScratchImage LoadTexture(const std::string& filePath);

	// textureResourceの生成
	ID3D12Resource* CreateTextureResource(ID3D12Device* device, const DirectX::TexMetadata& metadata);

	// textureResourceにデータを転送する
	ID3D12Resource* UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages, ID3D12Device* device,ID3D12GraphicsCommandList* commandList);

	// textureを読んで転送し、shaderResourceViewを生成
	void TransferTexture(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, ID3D12DescriptorHeap* srvDescriptorHeap);

	// shaderResourceViewの生成
	void CreateShaderResourceView(ID3D12Device* device, ID3D12DescriptorHeap* srvDescriptorHeap, const DirectX::TexMetadata& metadata);

	// 解放処理
	void Release();

	// COMの終了処理
	void ComUninit();
private:
	DirectX::ScratchImage mipImages_;
	ID3D12Resource* textureResource_;
	ID3D12Resource* intermediateResource_;
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc_;
	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU_;
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU_;
};
