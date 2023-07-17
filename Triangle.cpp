#include "Triangle.h"
#include "WinApp.h"
#include "ConvertString.h"
#include <format>
#include <cassert>

Triangle::~Triangle() {
	//wvpResource_->Release();
	//materialResource_->Release();
	//vertexResource_->Release();
}

const Microsoft::WRL::ComPtr<ID3D12Resource> Triangle::CreateBufferResource(const Microsoft::WRL::ComPtr<ID3D12Device>& device, size_t sizeInBytes) {
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
	hr = device.Get()->CreateCommittedResource(&uploadHeapProperties, D3D12_HEAP_FLAG_NONE,
		&vertexResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertexResource));
	assert(SUCCEEDED(hr));

	return vertexResource;
}

void Triangle::CreateVertexResource() {
	vertexResource_ = CreateBufferResource(directXCommon_->GetDevice(), sizeof(VertexData) * 3);
}

void Triangle::CreateVertexBufferView() {
	// リソースの先頭のアドレスから使う
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	// 使用するリソースのサイズは頂点3つ分のサイズ
	vertexBufferView_.SizeInBytes = sizeof(VertexData) * 3;
	// 1頂点当たりのサイズ
	vertexBufferView_.StrideInBytes = sizeof(VertexData);
}

void Triangle::CreateMaterialResource() {
	materialResource_ = CreateBufferResource(directXCommon_->GetDevice(), sizeof(Material));
	// マテリアルにデータを書き込む
	materialData_ = nullptr;
	// 書き込むためのアドレスを取得
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
}

void Triangle::CreateWvpResource() {
	// 1つ分のサイズを用意する
	wvpResource_ = CreateBufferResource(directXCommon_->GetDevice(), sizeof(TransformationMatrix));
	// 書き込むためのアドレスを取得
	wvpResource_->Map(0, nullptr, reinterpret_cast<void**>(&wvpData_));
	// 単位行列を書き込んでおく
	wvpData_->WVP = MakeIdentity4x4();
}

void Triangle::Initialize(DirectXCommon* directXCommon) {
	directXCommon_ = directXCommon;

	CreateVertexResource();

	CreateMaterialResource();

	CreateWvpResource();

	CreateVertexBufferView();

	// 書き込むためのアドレスを取得
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));

	// 三角形の情報
	transform_ = {
	{1.0f,1.0f,1.0f},
	{0.0f,0.0f,0.0f},
	{0.0f,0.0f,0.0f}
	};
	uvTransform_ = {
	{1.0f,1.0f,1.0f},
	{0.0f,0.0f,0.0f},
	{0.0f,0.0f,0.0f}
	};

	materialData_->uvTransform = MakeIdentity4x4();

	materialData_->enableLighting = false;
}

void Triangle::Draw(const Vector4& leftBottom, const Vector4& top, const Vector4& rightBottom, const Vector4& color, const Matrix4x4& transformationMatrixData) {
	uvTransformMatrix_ = MakeScaleMatrix(uvTransform_.scale);
	uvTransformMatrix_ = Multiply(uvTransformMatrix_, MakeRotateZMatrix(uvTransform_.rotate.z));
	uvTransformMatrix_ = Multiply(uvTransformMatrix_, MakeTranslateMatrix(uvTransform_.translate));
	materialData_->uvTransform = uvTransformMatrix_;

#pragma region 三角形の回転

	//transform_.rotate.y += 0.03f;
	wvpData_->World = MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	wvpData_->World = Multiply(wvpData_->World,transformationMatrixData);
	wvpData_->WVP = wvpData_->World;

#pragma endregion

	// 左下
	vertexData_[0].position = leftBottom;
	vertexData_[0].texcoord = { 0.0f,1.0f };
	// 上:
	vertexData_[1].position = top;
	vertexData_[1].texcoord = { 0.5f,0.0f };
	// 右下
	vertexData_[2].position = rightBottom;
	vertexData_[2].texcoord = {1.0f,1.0f};

	// 赤色にする
	materialData_->color = color;

	// コマンドを積む
	ID3D12GraphicsCommandList* commandList = directXCommon_->GetCommandList();
	commandList->IASetVertexBuffers(0, 1, &vertexBufferView_); // VBVを設定
	// 形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけば良い
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// マテリアルCBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	// wvp陽男のCBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(1, wvpResource_->GetGPUVirtualAddress());
	// DescriptorTableの設定
	commandList->SetGraphicsRootDescriptorTable(2, textureSrvHandleGPU_);
	// 描画(DrawCall/ドローコール)。3頂点で1つのインスタンス。インスタンスについては今後
	commandList->DrawInstanced(3, 1, 0, 0);
}
