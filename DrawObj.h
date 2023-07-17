#pragma once
#include "Matrix4x4.h"
#include "VertexData.h"
#include "Transform.h"
#include "TransformationMatrix.h"
#include "Material.h"
#include "MaterialData.h"
#include <d3d12.h>
#include "TextureManager.h"

class DrawObj
{
public:
	ID3D12Resource* CreateBufferResource(ID3D12Device* device, size_t sizeInBytes);

	void CreateVertexResource(ID3D12Device* device);

	void CreateVertexBufferView();

	void CreateMaterialResource(ID3D12Device* device);

	void CreateWvpResource(ID3D12Device* device);

	ModelData LoadObjFile(const std::string& directoryPath, const std::string& filename);

	void Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* commandList);

	void Draw(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, D3D12_GPU_DESCRIPTOR_HANDLE* textureSrvHandleGPU, const Matrix4x4& transformationMatrixData, ID3D12Resource* directionalLightResource);

	void Release();

public:
	// Material
	Material* materialData_;
	ID3D12Resource* materialResource_;
	Transform uvTransform_;
	Matrix4x4 uvTransformMatrix_;
	// Vertex
	ID3D12Resource* vertexResource_;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;
	VertexData* vertexData_;
	// カメラ
	ID3D12Resource* transformationMatrixResource_;
	TransformationMatrix* transformationMatrixData_;
	Transform transform_;
	Matrix4x4 viewMatrix_;
	Matrix4x4 projectionMatrix_;
	Matrix4x4 worldViewProjectionMatrix_;

	ModelData modelData_;
};