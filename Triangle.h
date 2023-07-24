#pragma once
#include <string>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxgidebug.h>
#include "DirIectXCommon.h"
#include "Vector4.h"

// 三角形の頂点
struct TriangleVertices {
	Vector4 left;
	Vector4 top;
	Vector4 right;
};

class Triangle
{
public:
	Triangle(Vector4 left, Vector4 top, Vector4 right);
	~Triangle() = default;

	ID3D12Resource* CreateBufferResource(ID3D12Device* device, size_t sizeInBytes);

	// VertexResourceの生成
	void CreateVertexResource();

	// VertexBufferViewの生成
	void CreateVertexBufferView();

	// 初期化
	void Initialize(DirectXCommon* directXCommon);

	// 三角形描画
	void Draw();

	// 解放処理
	void Release();

private:
	DirectXCommon* directXCommon_;
	ID3D12Resource* vertexResource_;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;
	Vector4* vertexData_;
	TriangleVertices vertex_;
};

