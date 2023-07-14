#include "TextureManager.h"
#include "ConvertString.h"
#include "WinApp.h"
#include <vector>
#include <cassert>
#define _USE_MATH_DEFINES
#include <math.h>
#include "ImGuiManager.h"

D3D12_CPU_DESCRIPTOR_HANDLE TextureManager::GetCPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index) {
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (descriptorSize * index);
	return handleCPU;
}

D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::GetGPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index) {
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (descriptorSize * index);
	return handleGPU;
}

ID3D12Resource* TextureManager::CreateBufferResource(ID3D12Device* device, size_t sizeInBytes) {
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

void TextureManager::ComInit() {
	CoInitializeEx(0, COINIT_MULTITHREADED);
}

DirectX::ScratchImage TextureManager::LoadTexture(const std::string& filePath) {
	// テクスチャファイルを読んでプログラムで扱えるようにする
	DirectX::ScratchImage image{};
	std::wstring filePathW = ConvertString(filePath);
	HRESULT hr = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
	assert(SUCCEEDED(hr));

	// ミニマップの生成
	DirectX::ScratchImage mipImages{};
	hr = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 0, mipImages);
	assert(SUCCEEDED(hr));

	// ミニマップ付きのデータを返す
	return mipImages;
}

ID3D12Resource* TextureManager::CreateTextureResource(ID3D12Device* device, const DirectX::TexMetadata& metadata) {
	// metadataをもとにResourceの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Width = UINT(metadata.width);
	resourceDesc.Height = UINT(metadata.height);
	resourceDesc.MipLevels = UINT16(metadata.mipLevels);
	resourceDesc.DepthOrArraySize = UINT16(metadata.arraySize);
	resourceDesc.Format = metadata.format;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION(metadata.dimension);

	// 利用するHeapの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;

	// Resourceの生成
	ID3D12Resource* resource = nullptr;
	HRESULT hr = device->CreateCommittedResource(
		&heapProperties,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&resource)
	);
	assert(SUCCEEDED(hr));
	return resource;
}

[[nodiscard]]
ID3D12Resource* TextureManager::UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages, ID3D12Device* device, ID3D12GraphicsCommandList* commandList) {
	std::vector<D3D12_SUBRESOURCE_DATA> subresources;
	DirectX::PrepareUpload(device, mipImages.GetImages(), mipImages.GetImageCount(), mipImages.GetMetadata(), subresources);
	uint64_t intermediateSize = GetRequiredIntermediateSize(texture, 0, UINT(subresources.size()));
	ID3D12Resource* intermediateResource = CreateBufferResource(device, intermediateSize);
	UpdateSubresources(commandList, texture, intermediateResource, 0, 0, UINT(subresources.size()), subresources.data());
	// textureへの転送後は利用できるように、D3D12_RESOURCE_STATE_COPY_DESTからD3D12_RESOURCE_STATE_GENERIC_READへResourceStateを変更する
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = texture;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;
	commandList->ResourceBarrier(1, &barrier);
	return intermediateResource;
}

void TextureManager::TransferTexture(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, ID3D12DescriptorHeap* srvDescriptorHeap) {
	mipImages_ = LoadTexture("resources/uvChecker.png");
	const DirectX::TexMetadata& metadata = mipImages_.GetMetadata();
	textureResource_ = CreateTextureResource(device, metadata);
	intermediateResource_ = UploadTextureData(textureResource_, mipImages_, device, commandList);

	// DescriptorSizeを取得
	const uint32_t descriptorSizeSRV = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	// metaDataをもとにSRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = UINT(metadata.mipLevels);
	// SRVを作成するDescriptorHeapの場所を決める
	textureSrvHandleCPU_ = GetCPUDescriptorHandle(srvDescriptorHeap, descriptorSizeSRV, 1);
	textureSrvHandleGPU_ = GetGPUDescriptorHandle(srvDescriptorHeap, descriptorSizeSRV, 1);
	// SRVの生成
	device->CreateShaderResourceView(textureResource_, &srvDesc, textureSrvHandleCPU_);

	mipImages2_ = LoadTexture("resources/monsterBall.png");
	const DirectX::TexMetadata& metadata2 = mipImages2_.GetMetadata();
	textureResource2_ = CreateTextureResource(device, metadata2);
	intermediateResource2_ = UploadTextureData(textureResource2_, mipImages2_, device, commandList);

	// metaDataをもとにSRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc2{};
	srvDesc2.Format = metadata2.format;
	srvDesc2.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc2.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc2.Texture2D.MipLevels = UINT(metadata2.mipLevels);
	textureSrvHandleCPU2_ = GetCPUDescriptorHandle(srvDescriptorHeap, descriptorSizeSRV, 2);
	textureSrvHandleGPU2_ = GetGPUDescriptorHandle(srvDescriptorHeap, descriptorSizeSRV, 2);
	// SRVの生成
	device->CreateShaderResourceView(textureResource2_, &srvDesc2, textureSrvHandleCPU2_);
}

ID3D12Resource* TextureManager::CreateDepthStencilTextureResource(ID3D12Device* device, int32_t width, int32_t height) {
	// 生成するResourceの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Width = width;									  // Textureの幅
	resourceDesc.Height = height;								  // Textureの高さ
	resourceDesc.MipLevels = 1;									  // mipmapの数
	resourceDesc.DepthOrArraySize = 1;							  // 奥行き or 配列Textureの配列数
	resourceDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;		  // DepthStencilとして利用可能なフォーマット
	resourceDesc.SampleDesc.Count = 1;							  // サンプリングカウント。1固定
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;  // 2次元
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL; // DepthStencilとして使う通知

	// 利用するHeapの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT; // VRAM上に作る

	// 深度値のクリア設定
	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f; // 1.0f(最大値)でクリア
	depthClearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // フォーマット。Resourceと合わせる

	// Resourceの生成
	ID3D12Resource* resource = nullptr;
	HRESULT hr = device->CreateCommittedResource(
		&heapProperties,				  // Heapの設定
		D3D12_HEAP_FLAG_NONE,			  // Heapの特殊な設定。特になし
		&resourceDesc,					  // Resourceの設定
		D3D12_RESOURCE_STATE_DEPTH_WRITE, // 深度値を書き込む状態にしておく
		&depthClearValue,				  // Clear最適値
		IID_PPV_ARGS(&resource)			  // 作成するResourceポインタへのポインタ
	);

	assert(SUCCEEDED(hr));

	return resource;
}

ID3D12DescriptorHeap* TextureManager::CreateDsvDescriptorHeap(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible) {
	HRESULT hr;
	ID3D12DescriptorHeap* descriptorHeap;
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescriptorHeapDesc{};
	rtvDescriptorHeapDesc.Type = heapType; // レンダーターゲットビュー用
	rtvDescriptorHeapDesc.NumDescriptors = numDescriptors; // ダブルバッファ用に2つ。多くても別に構わない
	rtvDescriptorHeapDesc.Flags = shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	hr = device->CreateDescriptorHeap(&rtvDescriptorHeapDesc, IID_PPV_ARGS(&descriptorHeap));
	// DiscriptorHeapが作れなかったので起動できない
	assert(SUCCEEDED(hr));

	return descriptorHeap;
}

void TextureManager::CreateDepthStencilView(ID3D12Device* device) {
	depthStencilResource_ = CreateDepthStencilTextureResource(device, WinApp::kClientWidth_, WinApp::kClientHeight_);

	dsvDescriptorHeap_ = CreateDsvDescriptorHeap(device, D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1, false);
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;

	const uint32_t descriptorSizeDSV = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	device->CreateDepthStencilView(depthStencilResource_, &dsvDesc, GetCPUDescriptorHandle(dsvDescriptorHeap_, descriptorSizeDSV, 0));
}

void TextureManager::SettingDepthStencilState() {
	// Depthの機能を有効化する
	depthStencilDesc_.DepthEnable = true;
	// 書き込みをします
	depthStencilDesc_.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	// 比較関数はLessEqual。つまり、近ければ描画される
	depthStencilDesc_.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
}

void TextureManager::CreateVertexResource(ID3D12Device* device) {
	vertexResourceSprit_ = CreateBufferResource(device, sizeof(VertexData) * 6);

	vertexResourceSphere_ = CreateBufferResource(device, sizeof(VertexData) * vertexIndex);
}

void TextureManager::CreateVertexBufferView() {
	// リソースの先頭のアドレスから使う
	vertexBufferViewSprite_.BufferLocation = vertexResourceSprit_->GetGPUVirtualAddress();
	// 使用するリソースのサイズは頂点3つ分のサイズ
	vertexBufferViewSprite_.SizeInBytes = sizeof(VertexData) * 6;
	// 1頂点当たりのサイズ
	vertexBufferViewSprite_.StrideInBytes = sizeof(VertexData);

	// リソースの先頭のアドレスから使う
	vertexBufferViewSphere_.BufferLocation = vertexResourceSphere_->GetGPUVirtualAddress();
	// 使用するリソースのサイズは頂点3つ分のサイズ
	vertexBufferViewSphere_.SizeInBytes = sizeof(VertexData) * vertexIndex;
	// 1頂点当たりのサイズ
	vertexBufferViewSphere_.StrideInBytes = sizeof(VertexData);
}

void TextureManager::CreateMaterialResource(ID3D12Device* device) {
	materialResource_ = CreateBufferResource(device, sizeof(Material));
	// マテリアルにデータを書き込む
	materialData_ = nullptr;
	// 書き込むためのアドレスを取得
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));

	materialResourceSphere_ = CreateBufferResource(device, sizeof(Material));
	// マテリアルにデータを書き込む
	materialDataSphere_ = nullptr;
	// 書き込むためのアドレスを取得
	materialResourceSphere_->Map(0, nullptr, reinterpret_cast<void**>(&materialDataSphere_));
}

void TextureManager::CreateWvpResource(ID3D12Device* device) {
	// 1つ分のサイズを用意する
	transformationMatrixResourceSprite_ = CreateBufferResource(device, sizeof(TransformationMatrix));
	// 書き込むためのアドレスを取得
	transformationMatrixResourceSprite_->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixDataSprite_));
	// 単位行列を書き込んでおく
	transformationMatrixDataSprite_->WVP = MakeIdentity4x4();

	// 1つ分のサイズを用意する
	wvpResourceSphere_ = CreateBufferResource(device, sizeof(TransformationMatrix));
	// 書き込むためのアドレスを取得
	wvpResourceSphere_->Map(0, nullptr, reinterpret_cast<void**>(&wvpDataSphere_));
	// 単位行列を書き込んでおく
	wvpDataSphere_->WVP = MakeIdentity4x4();
}

void TextureManager::CreateDirectionalResource(ID3D12Device* device) {
	directionalLightResource_ = CreateBufferResource(device, sizeof(DirectionalLight));
	// マテリアルにデータを書き込む
	directionalLightData_ = nullptr;
	// 書き込むためのアドレスを取得
	directionalLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData_));
}

void TextureManager::Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, ID3D12DescriptorHeap* srvDescriptorHeap) {
	TransferTexture(device, commandList, srvDescriptorHeap);

	CreateDepthStencilView(device);

	CreateVertexResource(device);

	CreateMaterialResource(device);

	CreateWvpResource(device);

	CreateVertexBufferView();

	CreateDirectionalResource(device);

	// 書き込むためのアドレスを取得
	vertexResourceSprit_->Map(0, nullptr, reinterpret_cast<void**>(&vertexDataSprite_));
	vertexResourceSphere_->Map(0, nullptr, reinterpret_cast<void**>(&vertexDataSphere_));

	transformSprite_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	transformSphere_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };

	materialData_->enableLighting = false;
	materialDataSphere_->enableLighting = true;

	directionalLightData_->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	directionalLightData_->direction = { 0.0f, -1.0f, 0.0f };
	directionalLightData_->intensity = 1.0f;
}

void TextureManager::DrawSprite(ID3D12Device* device, ID3D12GraphicsCommandList* commandList) {
	transformationMatrixDataSprite_->World = MakeAffineMatrix(transformSprite_.scale, transformSprite_.rotate, transformSprite_.translate);
	viewMatrixSprite_ = MakeIdentity4x4();
	projectionMatrixSprite_ = MakeOrthographicMatrix(0.0f, 0.0f, float(1280), float(720), 0.0f, 100.0f);
	worldViewProjectionMatrixSprite_ = Multiply(transformationMatrixDataSprite_->World, Multiply(viewMatrixSprite_, projectionMatrixSprite_));
	transformationMatrixDataSprite_->WVP = worldViewProjectionMatrixSprite_;

	// 一枚目の三角形
	vertexDataSprite_[0].position = { 0.0f, 360.0f, 0.0f, 1.0f };// 左下
	vertexDataSprite_[0].texcoord = { 0.0f,1.0f };
	vertexDataSprite_[0].normal = { 0.0f,0.0f,-1.0f };
	vertexDataSprite_[1].position = { 0.0f, 0.0f, 0.0f, 1.0f };// 左上
	vertexDataSprite_[1].texcoord = { 0.0f,0.0f };
	vertexDataSprite_[1].normal = { 0.0f,0.0f,-1.0f };
	vertexDataSprite_[2].position = { 640.0f, 360.0f, 0.0f, 1.0f };// 右下
	vertexDataSprite_[2].texcoord = { 1.0f,1.0f };
	vertexDataSprite_[2].normal = { 0.0f,0.0f,-1.0f };
	// 二枚目の三角形
	vertexDataSprite_[3].position = { 0.0f, 0.0f, 0.0f, 1.0f };// 左上
	vertexDataSprite_[3].texcoord = { 0.0f,0.0f };
	vertexDataSprite_[3].normal = { 0.0f,0.0f,-1.0f };
	vertexDataSprite_[4].position = { 640.0f, 0.0f, 0.0f, 1.0f };// 右上
	vertexDataSprite_[4].texcoord = { 1.0f,0.0f };
	vertexDataSprite_[4].normal = { 0.0f,0.0f,-1.0f };
	vertexDataSprite_[5].position = { 640.0f, 360.0f, 0.0f, 1.0f };// 右下
	vertexDataSprite_[5].texcoord = { 1.0f,1.0f };
	vertexDataSprite_[5].normal = { 0.0f,0.0f,-1.0f };

	materialData_->color = { 1.0f,1.0f,1.0f,1.0f };

	// コマンドを積む
	commandList->IASetVertexBuffers(0, 1, &vertexBufferViewSprite_); // VBVを設定
	// 形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけば良い
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// マテリアルCBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	// wvp陽男のCBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(1, transformationMatrixResourceSprite_->GetGPUVirtualAddress());
	commandList->SetGraphicsRootConstantBufferView(3, directionalLightResource_->GetGPUVirtualAddress());
	// DescriptorTableの設定
	commandList->SetGraphicsRootDescriptorTable(2, textureSrvHandleGPU_);
	// 描画(DrawCall/ドローコール)。3頂点で1つのインスタンス。インスタンスについては今後
	commandList->DrawInstanced(6, 1, 0, 0);
}

void TextureManager::DrawSphere(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, const Matrix4x4& transformationMatrixData) {
	// カメラ
	transformSphere_.rotate.y += 0.006f;
	wvpDataSphere_->World = MakeAffineMatrix(transformSphere_.scale, transformSphere_.rotate, transformSphere_.translate);
	wvpDataSphere_->WVP = Multiply(wvpDataSphere_->World, transformationMatrixData);
	wvpDataSphere_->World = MakeIdentity4x4();

	const float kLonEvery = 2.0f * float(M_PI) / float(kSubdivision);//経度分割1つ分の角度
	const float kLatEvery = float(M_PI) / float(kSubdivision);//緯度分割1つ分の角度
	// 緯度の方向に分割
	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		float lat = float(-M_PI) / 2.0f + kLatEvery * latIndex;
		// 経度の方向に分割しながら線を描く
		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			uint32_t start = (latIndex * kSubdivision + lonIndex) * 6;
			float lon = lonIndex * kLonEvery;
			// 頂点データを入力する。
#pragma region 1枚目
			// 基準点a
			vertexDataSphere_[start].position.x = cos(lat) * cos(lon);
			vertexDataSphere_[start].position.y = sin(lat);
			vertexDataSphere_[start].position.z = cos(lat) * sin(lon);
			vertexDataSphere_[start].position.w = 1.0f;
			vertexDataSphere_[start].texcoord = { float(lonIndex) / float(kSubdivision) ,1.0f - float(latIndex) / float(kSubdivision) };
			vertexDataSphere_[start].normal.x = vertexDataSphere_[start].position.x;
			vertexDataSphere_[start].normal.y = vertexDataSphere_[start].position.y;
			vertexDataSphere_[start].normal.z = vertexDataSphere_[start].position.z;
			// b
			vertexDataSphere_[start + 1].position.x = cos(lat + kLatEvery) * cos(lon);
			vertexDataSphere_[start + 1].position.y = sin(lat + kLatEvery);
			vertexDataSphere_[start + 1].position.z = cos(lat + kLatEvery) * sin(lon);
			vertexDataSphere_[start + 1].position.w = 1.0f;
			vertexDataSphere_[start + 1].texcoord = { vertexDataSphere_[start].texcoord.x,vertexDataSphere_[start].texcoord.y - (1.0f / kSubdivision) };
			vertexDataSphere_[start + 1].normal.x = vertexDataSphere_[start + 1].position.x;
			vertexDataSphere_[start + 1].normal.y = vertexDataSphere_[start + 1].position.y;
			vertexDataSphere_[start + 1].normal.z = vertexDataSphere_[start + 1].position.z;
			// c
			vertexDataSphere_[start + 2].position.x = cos(lat) * cos(lon + kLonEvery);
			vertexDataSphere_[start + 2].position.y = sin(lat);
			vertexDataSphere_[start + 2].position.z = cos(lat) * sin(lon + kLonEvery);
			vertexDataSphere_[start + 2].position.w = 1.0f;
			vertexDataSphere_[start + 2].texcoord = { vertexDataSphere_[start].texcoord.x + (1.0f / (float)kSubdivision),vertexDataSphere_[start].texcoord.y };
			vertexDataSphere_[start + 2].normal.x = vertexDataSphere_[start + 2].position.x;
			vertexDataSphere_[start + 2].normal.y = vertexDataSphere_[start + 2].position.y;
			vertexDataSphere_[start + 2].normal.z = vertexDataSphere_[start + 2].position.z;

#pragma endregion

#pragma region 2枚目

			// b
			vertexDataSphere_[start + 3].position.x = cos(lat + kLatEvery) * cos(lon);
			vertexDataSphere_[start + 3].position.y = sin(lat + kLatEvery);
			vertexDataSphere_[start + 3].position.z = cos(lat + kLatEvery) * sin(lon);
			vertexDataSphere_[start + 3].position.w = 1.0f;
			vertexDataSphere_[start + 3].texcoord = { vertexDataSphere_[start].texcoord.x,vertexDataSphere_[start].texcoord.y - (1.0f / (float)kSubdivision) };
			vertexDataSphere_[start + 3].normal.x = vertexDataSphere_[start + 3].position.x;
			vertexDataSphere_[start + 3].normal.y = vertexDataSphere_[start + 3].position.y;
			vertexDataSphere_[start + 3].normal.z = vertexDataSphere_[start + 3].position.z;
			// d
			vertexDataSphere_[start + 4].position.x = cos(lat + kLatEvery) * cos(lon + kLonEvery);
			vertexDataSphere_[start + 4].position.y = sin(lat + kLatEvery);
			vertexDataSphere_[start + 4].position.z = cos(lat + kLatEvery) * sin(lon + kLonEvery);
			vertexDataSphere_[start + 4].position.w = 1.0f;
			vertexDataSphere_[start + 4].texcoord = { vertexDataSphere_[start].texcoord.x + (1.0f / (float)kSubdivision),vertexDataSphere_[start].texcoord.y - (1.0f / (float)kSubdivision) };
			vertexDataSphere_[start + 4].normal.x = vertexDataSphere_[start + 4].position.x;
			vertexDataSphere_[start + 4].normal.y = vertexDataSphere_[start + 4].position.y;
			vertexDataSphere_[start + 4].normal.z = vertexDataSphere_[start + 4].position.z;
			// c
			vertexDataSphere_[start + 5].position.x = cos(lat) * cos(lon + kLonEvery);
			vertexDataSphere_[start + 5].position.y = sin(lat);
			vertexDataSphere_[start + 5].position.z = cos(lat) * sin(lon + kLonEvery);
			vertexDataSphere_[start + 5].position.w = 1.0f;
			vertexDataSphere_[start + 5].texcoord = { vertexDataSphere_[start].texcoord.x + (1.0f / (float)kSubdivision),vertexDataSphere_[start].texcoord.y };
			vertexDataSphere_[start + 5].normal.x = vertexDataSphere_[start + 5].position.x;
			vertexDataSphere_[start + 5].normal.y = vertexDataSphere_[start + 5].position.y;
			vertexDataSphere_[start + 5].normal.z = vertexDataSphere_[start + 5].position.z;

#pragma endregion

		}
	}

	materialDataSphere_->color = { 1.0f,1.0f,1.0f,1.0f };

	ImGui::Text("Sphere");
	ImGui::Checkbox("useMonsterBall", &useMonsterBall_);
	ImGui::SliderFloat3("Sphere.Rotate", &transformSphere_.rotate.x, -1, 1);
	ImGui::SliderFloat3("Sphere.Transform", &transformSphere_.translate.x, -2,2);

	ImGui::Text("Ligting");
	ImGui::SliderFloat3("Lighting.direction", &directionalLightData_->direction.x, -3, 3);

	// コマンドを積む
	commandList->IASetVertexBuffers(0, 1, &vertexBufferViewSphere_); // VBVを設定
	// 形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけば良い
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// DescriptorTableの設定
	commandList->SetGraphicsRootDescriptorTable(2, useMonsterBall_ ? textureSrvHandleGPU2_ : textureSrvHandleGPU_);

	// wvpのCBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(1, wvpResourceSphere_->GetGPUVirtualAddress());

	commandList->SetGraphicsRootConstantBufferView(3, directionalLightResource_->GetGPUVirtualAddress());

	// マテリアルCBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(0, materialResourceSphere_->GetGPUVirtualAddress());
	commandList->DrawInstanced(vertexIndex, 1, 0, 0);
}

void TextureManager::Release() {
	wvpResourceSphere_->Release();
	transformationMatrixResourceSprite_->Release();
	materialResourceSphere_->Release();
	materialResource_->Release();
	vertexResourceSprit_->Release();
	vertexResourceSphere_->Release();
	textureResource_->Release();
	textureResource2_->Release();
	intermediateResource_->Release();
	intermediateResource2_->Release();
	depthStencilResource_->Release();
	dsvDescriptorHeap_->Release();
	directionalLightResource_->Release();
}

void TextureManager::ComUninit() {
	CoUninitialize();
}
