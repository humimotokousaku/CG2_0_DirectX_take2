#include "Sprite.h"
#include "ImGuiManager.h"
#include <cassert>

Microsoft::WRL::ComPtr<ID3D12Resource> Sprite::CreateBufferResource(const Microsoft::WRL::ComPtr<ID3D12Device>& device, size_t sizeInBytes) {
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

	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource;
	// 実際に頂点リソースを作る
	hr = device.Get()->CreateCommittedResource(&uploadHeapProperties, D3D12_HEAP_FLAG_NONE,
		&vertexResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(vertexResource.GetAddressOf()));
	assert(SUCCEEDED(hr));

	return vertexResource;
}

void Sprite::CreateVertexResource(const Microsoft::WRL::ComPtr<ID3D12Device>& device) {
	vertexResource_ = CreateBufferResource(device.Get(), sizeof(VertexData) * 4).Get();
}

void Sprite::CreateVertexBufferView() {
	// リソースの先頭のアドレスから使う
	vertexBufferView_.BufferLocation = vertexResource_.Get()->GetGPUVirtualAddress();
	// 使用するリソースのサイズは頂点3つ分のサイズ
	vertexBufferView_.SizeInBytes = sizeof(VertexData) * 4;
	// 1頂点当たりのサイズ
	vertexBufferView_.StrideInBytes = sizeof(VertexData);
}

void Sprite::CreateMaterialResource(const Microsoft::WRL::ComPtr<ID3D12Device>& device) {
	materialResource_ = CreateBufferResource(device, sizeof(Material)).Get();
	// マテリアルにデータを書き込む
	materialData_ = nullptr;
	// 書き込むためのアドレスを取得
	materialResource_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
}

void Sprite::CreateWvpResource(const Microsoft::WRL::ComPtr <ID3D12Device>& device) {
	// 1つ分のサイズを用意する
	transformationMatrixResource_ = CreateBufferResource(device.Get(), sizeof(TransformationMatrix)).Get();
	// 書き込むためのアドレスを取得
	transformationMatrixResource_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixData_));
	// 単位行列を書き込んでおく
	transformationMatrixData_->WVP = MakeIdentity4x4();
}

Sprite::~Sprite() {

}

void Sprite::Initialize(const Microsoft::WRL::ComPtr<ID3D12Device>& device, const Microsoft::WRL::ComPtr <ID3D12GraphicsCommandList>& commandList) {
	CreateVertexResource(device.Get());
	indexResource_ = CreateBufferResource(device.Get(), sizeof(uint32_t) * 6).Get();
	indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));

	CreateMaterialResource(device.Get());

	CreateWvpResource(device.Get());

	CreateVertexBufferView();
	indexBufferView_.BufferLocation = indexResource_.Get()->GetGPUVirtualAddress();
	indexBufferView_.SizeInBytes = sizeof(uint32_t) * 6;
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;

	// 書き込むためのアドレスを取得
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
	indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));

	transform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	uvTransform_ = {
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f}
	};

	// Lightingするか
	materialData_->enableLighting = false;

	// uvTransform行列の初期化
	materialData_->uvTransform = MakeIdentity4x4();
}

void Sprite::Draw(const Microsoft::WRL::ComPtr<ID3D12Device>& device, const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList, D3D12_GPU_DESCRIPTOR_HANDLE* textureSrvHandleGPU, const Microsoft::WRL::ComPtr <ID3D12Resource>& directionalLightResource) {
	uvTransformMatrix_ = MakeScaleMatrix(uvTransform_.scale);
	uvTransformMatrix_ = Multiply(uvTransformMatrix_, MakeRotateZMatrix(uvTransform_.rotate.z));
	uvTransformMatrix_ = Multiply(uvTransformMatrix_, MakeTranslateMatrix(uvTransform_.translate));
	materialData_->uvTransform = uvTransformMatrix_;

	transformationMatrixData_->World = MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	viewMatrix_ = MakeIdentity4x4();
	projectionMatrix_ = MakeOrthographicMatrix(0.0f, 0.0f, float(1280), float(720), 0.0f, 100.0f);
	worldViewProjectionMatrix_ = Multiply(transformationMatrixData_->World, Multiply(viewMatrix_, projectionMatrix_));
	transformationMatrixData_->WVP = worldViewProjectionMatrix_;

	// 矩形のデータ
	vertexData_[0].position = { 0.0f, 360.0f, 0.0f, 1.0f };// 左下
	vertexData_[0].texcoord = { 0.0f,1.0f };
	vertexData_[0].normal = { 0.0f,0.0f,-1.0f };
	vertexData_[1].position = { 0.0f, 0.0f, 0.0f, 1.0f };// 左上
	vertexData_[1].texcoord = { 0.0f,0.0f };
	vertexData_[1].normal = { 0.0f,0.0f,-1.0f };
	vertexData_[2].position = { 640.0f, 360.0f, 0.0f, 1.0f };// 右下
	vertexData_[2].texcoord = { 1.0f,1.0f };
	vertexData_[2].normal = { 0.0f,0.0f,-1.0f };
	vertexData_[3].position = { 640.0f, 0.0f, 0.0f, 1.0f };// 右上
	vertexData_[3].texcoord = { 1.0f,0.0f };
	vertexData_[3].normal = { 0.0f,0.0f,-1.0f };

	indexData_[0] = 0;
	indexData_[1] = 1;
	indexData_[2] = 2;
	indexData_[3] = 1;
	indexData_[4] = 3;
	indexData_[5] = 2;

	materialData_->color = { 1.0f,1.0f,1.0f,1.0f };

	ImGui::Text("uvTransform");
	ImGui::SliderFloat2("uvTransform.Translate", &uvTransform_.translate.x, -2, 2);
	ImGui::SliderFloat2("uvTransform.Scale", &uvTransform_.scale.x, -1, 1);
	ImGui::SliderAngle("uvTransform.Rotate.z", &uvTransform_.rotate.z);

	// コマンドを積む
	commandList.Get()->IASetVertexBuffers(0, 1, &vertexBufferView_); // VBVを設定

	// 形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけば良い
	commandList.Get()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	commandList.Get()->IASetIndexBuffer(&indexBufferView_);
	commandList.Get()->SetGraphicsRootConstantBufferView(3, directionalLightResource.Get()->GetGPUVirtualAddress());
	// マテリアルCBufferの場所を設定
	commandList.Get()->SetGraphicsRootConstantBufferView(0, materialResource_.Get()->GetGPUVirtualAddress());
	// wvp陽男のCBufferの場所を設定
	commandList.Get()->SetGraphicsRootConstantBufferView(1, transformationMatrixResource_.Get()->GetGPUVirtualAddress());

	// DescriptorTableの設定
	commandList.Get()->SetGraphicsRootDescriptorTable(2, textureSrvHandleGPU[0]);

	// 描画(DrawCall/ドローコール)。6頂点で1つのインスタンス
	commandList.Get()->DrawIndexedInstanced(6, 1, 0, 0, 0);
}

void Sprite::Release() {
	//transformationMatrixResource_->Release();
	//indexResource_->Release();
	//vertexResource_->Release();
	//materialResource_->Release();
}