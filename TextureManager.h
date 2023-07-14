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

class TextureManager
{
public:
	static D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index);
	static D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index);

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

	// vertexResourceの生成
	void CreateVertexResource(ID3D12Device* device);

	// vertexBufferViewの生成
	void CreateVertexBufferView();

	// materialResourceの生成
	void CreateMaterialResource(ID3D12Device* device);

	// wvpResourceの生成
	void CreateWvpResource(ID3D12Device* device);

	void CreateDirectionalResource(ID3D12Device* device);

	// スプライトの初期化
	void Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, ID3D12DescriptorHeap* srvDescriptorHeap);

	//	スプライトの描画(今は三角形のtextureと同じ変えたい場合はtextureSrvHandleGPUを変える必要あり)
	void DrawSprite(ID3D12Device* device, ID3D12GraphicsCommandList* commandList);

	// 球体の描画
	void DrawSphere(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, const Matrix4x4& transformationMatrixData);

	// 解放処理
	void Release();

	// COMの終了処理
	void ComUninit();
public:
	// Spriteで使っている画像はuvChecker.png
	// Sphereで使っている画像はuvChecker.pngとmonsterBall.png。初期に読み込んでいるのはmonsterBall.png

	// Sprite(textureSrvHandleGPUは三角形にも使用)
	DirectX::ScratchImage mipImages_;
	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU_;
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU_;
	ID3D12Resource* textureResource_;
	ID3D12Resource* intermediateResource_;
	// Sphere
	DirectX::ScratchImage mipImages2_;
	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU2_;
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU2_;
	ID3D12Resource* textureResource2_;
	ID3D12Resource* intermediateResource2_;

#pragma region Depth

	D3D12_DEPTH_STENCIL_DESC depthStencilDesc_;
	ID3D12Resource* depthStencilResource_;
	ID3D12DescriptorHeap* dsvDescriptorHeap_;

#pragma endregion

#pragma region Material
	// Spriteと三角形
	Material* materialData_;
	ID3D12Resource* materialResource_;
	// Sphere
	Material* materialDataSphere_;
	ID3D12Resource* materialResourceSphere_;

#pragma endregion

#pragma region Vertex
	// Sprite
	ID3D12Resource* vertexResourceSprit_;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferViewSprite_;
	VertexData* vertexDataSprite_;
	// Sphere
	ID3D12Resource* vertexResourceSphere_;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferViewSphere_;
	VertexData* vertexDataSphere_;

#pragma endregion

#pragma region カメラ
	// Sprite
	ID3D12Resource* transformationMatrixResourceSprite_;
	TransformationMatrix* transformationMatrixDataSprite_;
	Transform transformSprite_;
	//Matrix4x4 worldMatrixSprite_;
	Matrix4x4 viewMatrixSprite_;
	Matrix4x4 projectionMatrixSprite_;
	Matrix4x4 worldViewProjectionMatrixSprite_;
	// Sphere
	ID3D12Resource* wvpResourceSphere_;
	TransformationMatrix* wvpDataSphere_;
	Transform transformSphere_;
	//Matrix4x4 worldMatrixSphere_;
	Matrix4x4 viewMatrixSphere_;
	Matrix4x4 projectionMatrixSphere_;
	Matrix4x4 worldViewProjectionMatrixSphere_;

#pragma endregion

	ID3D12Resource* directionalLightResource_;
	DirectionalLight* directionalLightData_;

	const uint32_t kSubdivision = 16; //分割数
	uint32_t vertexIndex = kSubdivision * kSubdivision * 6;

	// Sphereの画像切り替え
	bool useMonsterBall_ = true;
};
