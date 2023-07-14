#pragma once
#include <string>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxgidebug.h>
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#include <dxgidebug.h>
#pragma comment(lib, "dxguid.lib")
#include <dxcapi.h>
#pragma comment(lib, "dxcompiler.lib")

#include "DirectXCommon.h"
#include "Vector4.h"
#include "Matrix4x4.h"
#include "Transform.h"
#include "VertexData.h"
#include "Camera.h"
#include "Material.h"
#include "TransformationMatrix.h"

class Triangle
{
public:
	// Getter
	ID3D12Resource* GetMaterialResource() { return materialResource_; }

	// Setter
	void SetTextureSrvHandleGPU(D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU) { textureSrvHandleGPU_ = textureSrvHandleGPU; }

	~Triangle();

	// Resource生成
	ID3D12Resource* CreateBufferResource(ID3D12Device* device,size_t sizeInBytes);

	// VertexResourceの生成
	void CreateVertexResource();

	// VertexBufferViewの生成
	void CreateVertexBufferView();

	// MaterialResourceの生成
	void CreateMaterialResource();

	// TransformationMatrix用のResourceを生成
	void CreateWvpResource();

	// 初期化
	void Initialize(DirectXCommon* directXCommon);

	// 三角形描画
	void Draw(const Vector4& leftBottom, const Vector4& top, const Vector4& rightBottom, const Vector4& color, const Matrix4x4& transformationMatrixData);
	
public:
	DirectXCommon* directXCommon_;
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU_;
	ID3D12Resource* vertexResource_;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;
	VertexData* vertexData_;
	ID3D12Resource* materialResource_;
	Material* materialData_;
	ID3D12Resource* wvpResource_;
	TransformationMatrix* wvpData_;
	Transform transform_;
};