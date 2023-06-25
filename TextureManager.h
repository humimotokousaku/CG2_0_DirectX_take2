#pragma once
#include "externals/DirectXTex/DirectXTex.h"
#include <d3d12.h>
#include <string>
#include "Vector2.h"
#include "Vector4.h"

struct VertexData {
	Vector4 position;
	Vector2 texcoord;
};

class TextureManager
{
public:
	void Initialize();

	DirectX::ScratchImage LoadTexture(const std::string& filePath);

	ID3D12Resource* CreateTextureResource(ID3D12Device* device, const DirectX::TexMetadata& metadata);

	void UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages);

	void Finalize();
};
