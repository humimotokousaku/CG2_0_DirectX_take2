#pragma once
#include "externals/DirectXTex/d3dx12.h"
#include "externals/DirectXTex/DirectXTex.h"
#include <string>

#include "Matrix4x4.h"
#include "VertexData.h"
#include "Transform.h"
#include "Triangle.h"
#include <d3d12.h>

class TextureManager
{
public:
	// Getter
	ID3D12DescriptorHeap* GetDsvDescriptorHeap() { return dsvDescriptorHeap_; }
	ID3D12Resource* GetDepthStencilResource() { return depthStencilResource_; }
	D3D12_DEPTH_STENCIL_DESC GetDepthStencilDesc() { return depthStencilDesc_; }
	D3D12_GPU_DESCRIPTOR_HANDLE GetTextureSrvHandleGPU() { return textureSrvHandleGPU_; }

	// COMの初期化
	void ComInit();

	// Textureを読む
	DirectX::ScratchImage LoadTexture(const std::string& filePath);

	// DirectX12のTextureResourceを作る
	ID3D12Resource* CreateTextureResource(ID3D12Device* device, const DirectX::TexMetadata& metadata);

	// 中間リソースの生成
	ID3D12Resource* CreateBufferResource(ID3D12Device* device, size_t sizeInBytes);

	// TextureResourceにデータを転送する
	ID3D12Resource* UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages, ID3D12Device* device,ID3D12GraphicsCommandList* commandList);

	// DepthStenciltextureの生成
	ID3D12Resource* CreateDepthStencilTextureResource(ID3D12Device* device, int32_t width, int32_t height);

	// dsvDescriptorHeapの生成
	ID3D12DescriptorHeap* CreateDsvDescriptorHeap(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible);
	
	// CPUで書き込む用のTextureResourceを作りコマンドを積む
	void CreateDepthStencilView(ID3D12Device* device);

	// DepthStencilStateの設定
	void SettingDepthStencilState();

	// textureを読んで転送する
	void TransferTexture(ID3D12Device* device, ID3D12GraphicsCommandList* commandList);

	void CreateDescriptorRange();

	void CreateDescriptorTable();

	// ShaderResourceViewを生成
	void CreateShaderResourceView(ID3D12Device* device, ID3D12DescriptorHeap* srvDescriptorHeap);





	void CreateVertexBufferViewSprite();
	// マテリアルリソース
	void CreateMaterialResource(ID3D12Device* device);
	// 矩形に画像を張るための頂点データとuv座標
	void VariableSpriteInitialize();

	void SpriteInitialize(ID3D12Device* device, D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU);

	void Draw(ID3D12Device* device);

	// 解放処理
	void Release();

	void Finalize();
public:
	ID3D12Resource* depthStencilResource_;
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc_;
	ID3D12DescriptorHeap* dsvDescriptorHeap_;
	DirectX::TexMetadata metadata_;
	DirectX::ScratchImage mipImages_;
	ID3D12Resource* textureResource_;
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc_;
	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU_;
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU_;
	ID3D12Resource* intermediateResource_;
	//D3D12_HEAP_PROPERTIES heapProperties_;
	D3D12_DESCRIPTOR_RANGE descriptorRange_[1];
	D3D12_STATIC_SAMPLER_DESC staticSamplers_[1];

	DirectXCommon* directXCommon_;
	Triangle* triangle_;
	ID3D12Resource* vertexResourceSprit_;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferViewSprite_;
	VertexData* vertexDataSprite_;
	ID3D12Resource* transformationMatrixResourceSprite_;
	Matrix4x4* transformationMatrixDataSprite_;
	Transform transformSprite_;
	Matrix4x4 worldMatrixSprite_;
	Matrix4x4 viewMatrixSprite_;
	Matrix4x4 projectionMatrixSprite_;
	Matrix4x4 worldViewProjectionMatrixSprite_;
	ID3D12Resource* materialResource_;
	Vector4* materialData_;
};
