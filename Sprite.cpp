#include "Sprite.h"
#include "WinApp.h"
#include <cassert>
#include "DirectXCommon.h"

//void Sprite::CreateVertexBufferViewSprite() {
//	// リソースの先頭のアッドレスから使う
//	vertexBufferViewSprite_.BufferLocation = vertexResourceSprit_->GetGPUVirtualAddress();
//	// 使用するリソースのサイズは頂点6つ分のサイズ
//	vertexBufferViewSprite_.SizeInBytes = sizeof(VertexData) * 6;
//	// 1頂点当たりのサイズ
//	vertexBufferViewSprite_.StrideInBytes = sizeof(VertexData);
//}
//
//ID3D12Resource* Sprite::CreateBufferResource(ID3D12Device* device, size_t sizeInBytes) {
//	HRESULT hr;
//	// 頂点リソース用のヒープの設定
//	D3D12_HEAP_PROPERTIES uploadHeapProperties{};
//	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD; // UploadHeapを使う
//	// 頂点リソースの設定
//	D3D12_RESOURCE_DESC vertexResourceDesc{};
//	// バッファソース。テクスチャの場合はまた別の設定をする
//	vertexResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
//	vertexResourceDesc.Width = sizeInBytes; // リソースのサイズ。今回はVector4を3頂点分
//	// バッファの場合はこれからは1にする決まり
//	vertexResourceDesc.Height = 1;
//	vertexResourceDesc.DepthOrArraySize = 1;
//	vertexResourceDesc.MipLevels = 1;
//	vertexResourceDesc.SampleDesc.Count = 1;
//	// バッファの場合はこれにする決まり
//	vertexResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
//
//	ID3D12Resource* vertexResource;
//	// 実際に頂点リソースを作る
//	hr = device->CreateCommittedResource(&uploadHeapProperties, D3D12_HEAP_FLAG_NONE,
//		&vertexResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertexResource));
//	assert(SUCCEEDED(hr));
//
//	return vertexResource;
//}
//
//void Sprite::CreateMaterialResource() {
//	materialResource_ = CreateBufferResource(directXCommon_->GetDevice(), sizeof(Vector4));
//	// マテリアルにデータを書き込む
//	//materialData_ = nullptr;
//	// 書き込むためのアドレスを取得
//	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
//}
//
//void Sprite::VariableSpriteInitialize() {
//	// 一枚目の三角形
//	vertexDataSprite_[0].position = { 0.0f, 360.0f, 0.0f, 1.0f };// 左下
//	vertexDataSprite_[0].texcoord = { 0.0f,1.0f };
//	vertexDataSprite_[1].position = { 0.0f, 0.0f, 0.0f, 1.0f };// 左上
//	vertexDataSprite_[1].texcoord = { 0.0f,0.0f };
//	vertexDataSprite_[2].position = { 640.0f, 360.0f, 0.0f, 1.0f };// 右下
//	vertexDataSprite_[2].texcoord = { 1.0f,1.0f };
//	// 二枚目の三角形
//	vertexDataSprite_[3].position = { 0.0f, 0.0f, 0.0f, 1.0f };// 左上
//	vertexDataSprite_[3].texcoord = { 0.0f,0.0f };
//	vertexDataSprite_[4].position = { 640.0f, 0.0f, 0.0f, 1.0f };// 右上
//	vertexDataSprite_[4].texcoord = { 1.0f,0.0f };
//	vertexDataSprite_[5].position = { 640.0f, 360.0f, 0.0f, 1.0f };// 右下
//	vertexDataSprite_[5].texcoord = { 1.0f,1.0f };
//}
//
//void Sprite::Initialize(D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU) {
//	// Sprite用のResource
//	vertexResourceSprit_ = CreateBufferResource(directXCommon_->GetDevice(), sizeof(VertexData) * 6);
//
//	CreateMaterialResource();
//
//	// Spriteの頂点データ
//	vertexResourceSprit_->Map(0, nullptr, reinterpret_cast<void**>(&vertexDataSprite_));
//
//	// wvpデータの生成
//	transformationMatrixResourceSprite_ = CreateBufferResource(directXCommon_->GetDevice(), sizeof(Matrix4x4));
//	transformationMatrixResourceSprite_->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixDataSprite_));
//	*transformationMatrixDataSprite_ = MakeIdentity4x4();
//
//	// vertexBufferViewの生成
//	CreateVertexBufferViewSprite();
//	
//	// 頂点データとuv座標
//	VariableSpriteInitialize();
//
//	transformSprite_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
//
//	textureSrvHandleGPU_ = textureSrvHandleGPU;
//}
//
//void Sprite::Draw() {
//	worldMatrixSprite_ = MakeAffineMatrix(transformSprite_.scale, transformSprite_.rotate, transformSprite_.translate);
//	viewMatrixSprite_ = MakeIdentity4x4();
//	projectionMatrixSprite_ = MakeOrthographicMatrix(0.0f, 0.0f, float(WinApp::kClientWidth_), float(WinApp::kClientHeight_), 0.0f, 100.0f);
//	worldViewProjectionMatrixSprite_ = Multiply(worldMatrixSprite_, Multiply(viewMatrixSprite_, projectionMatrixSprite_));
//	transformationMatrixDataSprite_ = &worldViewProjectionMatrixSprite_;
//
//	*materialData_ = { 1.0f,1.0f,1.0f,1.0f };
//
//	// コマンドを積む
//	ID3D12GraphicsCommandList* commandList = directXCommon_->GetCommandList();
//	commandList->IASetVertexBuffers(0, 1, &vertexBufferViewSprite_); // VBVを設定
//	// 形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけば良い
//	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//	// マテリアルCBufferの場所を設定
//	commandList->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
//	// wvp陽男のCBufferの場所を設定
//	commandList->SetGraphicsRootConstantBufferView(1, transformationMatrixResourceSprite_->GetGPUVirtualAddress());
//	// DescriptorTableの設定
//	commandList->SetGraphicsRootDescriptorTable(2, textureSrvHandleGPU_);
//	// 描画(DrawCall/ドローコール)。3頂点で1つのインスタンス。インスタンスについては今後
//	commandList->DrawInstanced(6, 1, 0, 0);
//}
//
//void Sprite::Release() {
//	transformationMatrixResourceSprite_->Release();
//	vertexResourceSprit_->Release();
//	materialResource_->Release();
//}