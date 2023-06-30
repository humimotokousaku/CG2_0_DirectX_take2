#pragma once
#include "externals/DirectXTex/d3dx12.h"
#include "externals/DirectXTex/DirectXTex.h"

#include <string>
#include "Triangle.h"

class TextureManager
{
public:
	void Initialize();

	DirectX::ScratchImage LoadTexture(const std::string& filePath);

	ID3D12Resource* CreateTextureResource(ID3D12Device* device, const DirectX::TexMetadata& metadata);

	ID3D12Resource* UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages, ID3D12Device* device,ID3D12GraphicsCommandList* commandList);

	//void TextureUpdate();

	void Finalize();
public:
	Triangle triangle_;
//	DirectXCommon* directXCommon_;
//	DirectX::ScratchImage mipImages_;
//	DirectX::TexMetadata metadata_;
//	ID3D12Resource* textureResource_;
//	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc_;
};
