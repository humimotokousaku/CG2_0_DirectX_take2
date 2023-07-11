#pragma once
#include "externals/DirectXTex/d3dx12.h"
#include "externals/DirectXTex/DirectXTex.h"
#include <string>

#include "Matrix4x4.h"
#include "VertexData.h"
#include "Transform.h"
#include <d3d12.h>

D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index);

D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index);

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
	void TransferTexture(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, ID3D12DescriptorHeap* srvDescriptorHeap);

	// ShaderResourceViewを生成
	void CreateShaderResourceView(ID3D12Device* device, ID3D12DescriptorHeap* srvDescriptorHeap, const DirectX::TexMetadata& metadata, D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandle);
	
	// vertexResourceの生成
	void CreateVertexResource(ID3D12Device* device);

	// vertexBufferViewの生成
	void CreateVertexBufferView();

	// materialResourceの生成
	void CreateMaterialResource(ID3D12Device* device);

	// wvpResourceの生成
	void CreateWvpResource(ID3D12Device* device);

	// スプライトの初期化
	void SpriteInitialize(ID3D12Device* device);
	
	//	スプライトの描画(今は三角形のtextureと同じ変えたい場合はtextureSrvHandleGPUを変える必要あり)
	void DrawSprite(ID3D12Device* device, ID3D12GraphicsCommandList* commandList);

	// 球体の描画
	void DrawSphere(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, const Matrix4x4& transformationMatrixData);

	// 解放処理
	void Release();

	// COMの終了処理
	void ComUninit();
public:

	D3D12_DEPTH_STENCIL_DESC depthStencilDesc_;

	DirectX::ScratchImage mipImages_;
	DirectX::ScratchImage mipImages2_;
	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU_;
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU_;
	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU2_;
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU2_;
	ID3D12Resource* depthStencilResource_;
	ID3D12DescriptorHeap* dsvDescriptorHeap_;
	ID3D12Resource* textureResource_;
	ID3D12Resource* textureResource2_;
	ID3D12Resource* transformationMatrixResourceSprite_;
	ID3D12Resource* materialResource_;
	ID3D12Resource* intermediateResource_;
	ID3D12Resource* intermediateResource2_;
	ID3D12Resource* vertexResourceSprit_;
	ID3D12Resource* wvpResourceSphere_;
	ID3D12Resource* vertexResourceSphere_;
	ID3D12Resource* materialResourceSphere_;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferViewSprite_;
	VertexData* vertexDataSprite_;

	Matrix4x4* transformationMatrixDataSprite_;
	Transform transformSprite_;
	Matrix4x4 worldMatrixSprite_;
	Matrix4x4 viewMatrixSprite_;
	Matrix4x4 projectionMatrixSprite_;
	Matrix4x4 worldViewProjectionMatrixSprite_;

	Vector4* materialData_;
	
	Transform transformSphere_;
	Matrix4x4 worldMatrixSphere_;
	Matrix4x4 viewMatrixSphere_;
	Matrix4x4 projectionMatrixSphere_;
	Matrix4x4 worldViewProjectionMatrixSphere_;

	Matrix4x4* wvpDataSphere_;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferViewSphere_;
	VertexData* vertexDataSphere_;

	Vector4* materialDataSphere_;
	const uint32_t kSubdivision = 16; //分割数
	const uint32_t kLatIndex = 16;
	const uint32_t kLonIndex = 16;
	uint32_t startIndex = (kLatIndex * kSubdivision + kLonIndex) * 6;

	bool useMonsterBall_ = true;
};
