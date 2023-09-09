#pragma once
#include "math/Matrix4x4.h"
#include "VertexData.h"
#include "Transform.h"
#include "base/WorldTransform.h"
#include "base/ViewProjection.h"
#include "Material.h"
#include "MaterialData.h"
#include <d3d12.h>

class Model
{public:

private:
	// Material
	Material* materialData_;
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_;
	Transform uvTransform_;
	Matrix4x4 uvTransformMatrix_;
	// Vertex
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;
	VertexData* vertexData_;
};

