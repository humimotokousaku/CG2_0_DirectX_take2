#pragma once
#include "Matrix4x4.h"
#include "VertexData.h"
#include "Transform.h"
#include "Triangle.h"
#include <d3d12.h>

class Sprite
{
public:
	void CreateVertexBufferViewSprite();

	ID3D12Resource* CreateBufferResource(ID3D12Device* device, size_t sizeInBytes);

	void CreateMaterialResource();

	void VariableSpriteInitialize();

	void Initialize(D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU);

	void Draw();

	void Release();

public:
	//DirectXCommon* directXCommon_;
	//Triangle* triangle_;
	//ID3D12Resource* vertexResourceSprit_;
	//D3D12_VERTEX_BUFFER_VIEW vertexBufferViewSprite_;
	//D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU_;
	//VertexData* vertexDataSprite_;
	//ID3D12Resource* transformationMatrixResourceSprite_;
	//Matrix4x4* transformationMatrixDataSprite_;
	//Transform transformSprite_;
	//Matrix4x4 worldMatrixSprite_;
	//Matrix4x4 viewMatrixSprite_;
	//Matrix4x4 projectionMatrixSprite_;
	//Matrix4x4 worldViewProjectionMatrixSprite_;
	//ID3D12Resource* materialResource_;
	//Vector4* materialData_;
};

