#pragma once
#include "externals/DirectXTex/DirectXTex.h"
#include <d3d12.h>
#include <string>
#include "Vector2.h"
#include "Vector4.h"
#include "VertexData.h"

class TextureManager
{
public:
	// Getter
	D3D12_GPU_DESCRIPTOR_HANDLE GetTextureSrvHandleGPU() {return textureSrvHandleGPU_;}

	void Initialize();

	DirectX::ScratchImage LoadTexture(const std::string& filePath);

	ID3D12Resource* CreateTextureResource(ID3D12Device* device, const DirectX::TexMetadata& metadata);

	void UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages);

	void TransferTexture(ID3D12Device* device);

	void CreateShaderResourceView(ID3D12Device* device, ID3D12DescriptorHeap* srvDescriptorHeap);

	void Release();

	void Finalize();

public:
	DirectX::TexMetadata metadata_;
	DirectX::ScratchImage mipImages_;
	ID3D12Resource* textureResource_;
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc_;
	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU_;
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU_;
	/*ID3D12Resource* vertexResourceSprit_;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferViewSprite_;
	VertexData* vertexDataSprite_;
	ID3D12Resource* transformationMatrixResourceSprite_;*/
	//Matrix4x4* transformationMatrixDataSprite_;
	//Transform transformSprite_;
	//Matrix4x4 worldMatrixSprite_;
	//Matrix4x4 viewMatrixSprite_;
	//Matrix4x4 projectionMatrixSprite_;
	//Matrix4x4 worldViewProjectionMatrixSprite_;
	/*ID3D12Resource* materialResource_;
	Vector4* materialData_;*/
};
