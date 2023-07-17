#pragma once
#include "externals/DirectXTex/d3dx12.h"
#include "externals/DirectXTex/DirectXTex.h"
#include <string>

#include "Matrix4x4.h"
#include "VertexData.h"
#include "Transform.h"
#include <d3d12.h>
#include "Material.h"
#include "TransformationMatrix.h"
#include "DirectionalLight.h"
#include "Light.h"
#include "ModelData.h"
#include "MaterialData.h"

class TextureManager
{
public:
	static D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index);
	static D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index);

	// Getter
	ID3D12DescriptorHeap* GetDsvDescriptorHeap() { return dsvDescriptorHeap_; }
	ID3D12Resource* GetDepthStencilResource() { return depthStencilResource_; }
	D3D12_DEPTH_STENCIL_DESC GetDepthStencilDesc() { return depthStencilDesc_; }
	D3D12_GPU_DESCRIPTOR_HANDLE* GetTextureSrvHandleGPU() { return textureSrvHandleGPU_; }
	ModelData GetModelData() { return modelData_; }

	// COMの初期化
	void ComInit();

	// Textureを読む
	DirectX::ScratchImage LoadTexture(const std::string& filePath);

	// objファイルの読み込み
	ModelData LoadObjFile(const std::string& directoryPath, const std::string& filename);

	// DirectX12のTextureResourceを作る
	ID3D12Resource* CreateTextureResource(ID3D12Device* device, const DirectX::TexMetadata& metadata);

	// 中間リソースの生成
	ID3D12Resource* CreateBufferResource(ID3D12Device* device, size_t sizeInBytes);

	// TextureResourceにデータを転送する
	ID3D12Resource* UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages, ID3D12Device* device, ID3D12GraphicsCommandList* commandList);

	// DepthStenciltextureの生成
	ID3D12Resource* CreateDepthStencilTextureResource(ID3D12Device* device, int32_t width, int32_t height);

	// dsvDescriptorHeapの生成
	ID3D12DescriptorHeap* CreateDsvDescriptorHeap(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible);

	// CPUで書き込む用のTextureResourceを作りコマンドを積む
	void CreateDepthStencilView(ID3D12Device* device);

	// DepthStencilStateの設定
	void SettingDepthStencilState();

	// textureを読んで転送する
	void TransferTexture(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, ID3D12DescriptorHeap* srvDescriptorHeap);

	// スプライトの初期化
	void Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, ID3D12DescriptorHeap* srvDescriptorHeap);

	// 解放処理
	void Release();

	// COMの終了処理
	void ComUninit();
public:
	// [0]はSpriteに使用しているuvChecker.png(textureSrvHandleGPUは三角形にも使用)[1]はSphereに使用しているmonsterBall.png
	DirectX::ScratchImage mipImages_[2];
	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU_[2];
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU_[2];
	ID3D12Resource* textureResource_[2];
	ID3D12Resource* intermediateResource_[2];
	// Depth
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc_;
	ID3D12Resource* depthStencilResource_;
	ID3D12DescriptorHeap* dsvDescriptorHeap_;

	ModelData modelData_;
};
