#include "DrawObj.h"
#include "Sphere.h"
#include "ImGuiManager.h"
#include <cassert>
#define _USE_MATH_DEFINES
#include <math.h>

ID3D12Resource* DrawObj::CreateBufferResource(ID3D12Device* device, size_t sizeInBytes) {
	HRESULT hr;
	// 頂点リソース用のヒープの設定
	D3D12_HEAP_PROPERTIES uploadHeapProperties{};
	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD; // UploadHeapを使う
	// 頂点リソースの設定
	D3D12_RESOURCE_DESC vertexResourceDesc{};
	// バッファソース。テクスチャの場合はまた別の設定をする
	vertexResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	vertexResourceDesc.Width = sizeInBytes; // リソースのサイズ。今回はVector4を3頂点分
	// バッファの場合はこれからは1にする決まり
	vertexResourceDesc.Height = 1;
	vertexResourceDesc.DepthOrArraySize = 1;
	vertexResourceDesc.MipLevels = 1;
	vertexResourceDesc.SampleDesc.Count = 1;
	// バッファの場合はこれにする決まり
	vertexResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	ID3D12Resource* vertexResource;
	// 実際に頂点リソースを作る
	hr = device->CreateCommittedResource(&uploadHeapProperties, D3D12_HEAP_FLAG_NONE,
		&vertexResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertexResource));
	assert(SUCCEEDED(hr));

	return vertexResource;
}

void DrawObj::CreateVertexResource(ID3D12Device* device) {
	vertexResource_ = CreateBufferResource(device, sizeof(VertexData) * modelData_.vertices.size());
}

void DrawObj::CreateVertexBufferView() {
	// リソースの先頭のアドレスから使う
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	// 使用するリソースのサイズは頂点3つ分のサイズ
	vertexBufferView_.SizeInBytes = sizeof(VertexData) * UINT(modelData_.vertices.size());
	// 1頂点当たりのサイズ
	vertexBufferView_.StrideInBytes = sizeof(VertexData);
}

void DrawObj::CreateMaterialResource(ID3D12Device* device) {
	materialResource_ = CreateBufferResource(device, sizeof(Material));
	// マテリアルにデータを書き込む
	materialData_ = nullptr;
	// 書き込むためのアドレスを取得
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
}

void DrawObj::CreateWvpResource(ID3D12Device* device) {
	// 1つ分のサイズを用意する
	transformationMatrixResource_ = CreateBufferResource(device, sizeof(TransformationMatrix));
	// 書き込むためのアドレスを取得
	transformationMatrixResource_->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixData_));
	// 単位行列を書き込んでおく
	transformationMatrixData_->WVP = MakeIdentity4x4();
}

void DrawObj::Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, ModelData modelData) {
	modelData_ = modelData;
	CreateVertexResource(device);

	CreateMaterialResource(device);

	CreateWvpResource(device);

	CreateVertexBufferView();

	// 書き込むためのアドレスを取得
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
	std::memcpy(vertexData_, modelData_.vertices.data(), sizeof(VertexData) * modelData_.vertices.size());

	transform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	uvTransform_ = {
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f}
	};

	// Lightingするか
	materialData_->enableLighting = true;

	// uvTransform行列の初期化
	materialData_->uvTransform = MakeIdentity4x4();
}

void DrawObj::Draw(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, D3D12_GPU_DESCRIPTOR_HANDLE* textureSrvHandleGPU, const Matrix4x4& transformationMatrixData, ID3D12Resource* directionalLightResource) {
	uvTransformMatrix_ = MakeScaleMatrix(uvTransform_.scale);
	uvTransformMatrix_ = Multiply(uvTransformMatrix_, MakeRotateZMatrix(uvTransform_.rotate.z));
	uvTransformMatrix_ = Multiply(uvTransformMatrix_, MakeTranslateMatrix(uvTransform_.translate));
	materialData_->uvTransform = uvTransformMatrix_;

	// カメラ
	//transform_.rotate.y += 0.006f;
	transformationMatrixData_->World = MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	transformationMatrixData_->WVP = Multiply(transformationMatrixData_->World, transformationMatrixData);
	transformationMatrixData_->World = MakeIdentity4x4();

	materialData_->color = { 1.0f,1.0f,1.0f,1.0f };

	ImGui::Text("objModel");
	ImGui::SliderFloat3(".Translate ", &transform_.translate.x, -2, 2);
	ImGui::SliderAngle(".Rotate.y ", &transform_.rotate.y);

	// コマンドを積む
	commandList->IASetVertexBuffers(0, 1, &vertexBufferView_); // VBVを設定
	// 形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけば良い
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// DescriptorTableの設定
	commandList->SetGraphicsRootDescriptorTable(2,textureSrvHandleGPU[1]);

	// wvpのCBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(1, transformationMatrixResource_->GetGPUVirtualAddress());

	commandList->SetGraphicsRootConstantBufferView(3, directionalLightResource->GetGPUVirtualAddress());

	// マテリアルCBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	commandList->DrawInstanced(UINT(modelData_.vertices.size()), 1, 0, 0);
}

void DrawObj::Release() {
	transformationMatrixResource_->Release();
	materialResource_->Release();
	vertexResource_->Release();
}