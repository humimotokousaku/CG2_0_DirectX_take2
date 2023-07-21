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
#include <wrl.h>

class Triangle
{
public:
	// Getter
	const Microsoft::WRL::ComPtr<ID3D12Resource> GetMaterialResource() { return materialResource_.Get(); }

	// Setter
	void SetTextureSrvHandleGPU(D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU) { textureSrvHandleGPU_ = textureSrvHandleGPU; }

	~Triangle();

	// Resource生成
	const Microsoft::WRL::ComPtr<ID3D12Resource> CreateBufferResource(const Microsoft::WRL::ComPtr<ID3D12Device>& device, size_t sizeInBytes);

	// VertexResourceの生成
	void CreateVertexResource(const Microsoft::WRL::ComPtr<ID3D12Device>& device);

	// VertexBufferViewの生成
	void CreateVertexBufferView();

	// MaterialResourceの生成
	void CreateMaterialResource(const Microsoft::WRL::ComPtr<ID3D12Device>& device);

	// TransformationMatrix用のResourceを生成
	void CreateWvpResource(const Microsoft::WRL::ComPtr<ID3D12Device>& device);

	// 初期化
	void Initialize(const Microsoft::WRL::ComPtr<ID3D12Device>& device, const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList);

	// 三角形描画
	void Draw(const Vector4& leftBottom, const Vector4& top, const Vector4& rightBottom, const Vector4& color, const Matrix4x4& transformationMatrixData, const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList);
	
public:
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU_;
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;
	VertexData* vertexData_;
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_;
	Material* materialData_;
	Microsoft::WRL::ComPtr<ID3D12Resource> wvpResource_;
	TransformationMatrix* wvpData_;
	Transform transform_;

	Transform uvTransform_;
	Matrix4x4 uvTransformMatrix_;
};