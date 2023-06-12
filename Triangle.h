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

#include "DirIectXCommon.h"
#include "Vector4.h"

class Triangle
{
public:

	~Triangle() = default;

	// Resource生成
	ID3D12Resource* CreateBufferResource(ID3D12Device* device, size_t sizeInBytes);

	// VertexResourceの生成
	void CreateVertexResource();

	// VertexBufferViewの生成
	void CreateVertexBufferView();

	// MaterialResourceの生成
	void CreateMaterialResource();

	// 初期化
	void Initialize(DirectXCommon* directXCommon);

	// 三角形描画
	void Draw(const Vector4& leftBottom, const Vector4& top, const Vector4& rightBottom);

public:
	DirectXCommon* directXCommon_;
	ID3D12Resource* vertexResource_;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;
	Vector4* vertexData_;

	ID3D12Resource* materialResource_;
	Vector4* materialData_;
};

