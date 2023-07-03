#include "MyEngine.h"
#include "ConvertString.h"
#include <format>
#include <cassert>

void MyEngine::DXCInitialize() {
	HRESULT hr;
	// dxCompilerの初期化
	hr = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils_));
	assert(SUCCEEDED(hr));
	hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler_));
	assert(SUCCEEDED(hr));

	// 現時点でincludeはしないが、includeに対応するために設定を行っておく
	hr = dxcUtils_->CreateDefaultIncludeHandler(&includeHandler_);
	assert(SUCCEEDED(hr));

	//DirectX::PrepareUpload()
}

IDxcBlob* MyEngine::CompileShader(
	// CompilerするShaderファイルへのパス
	const std::wstring& filePath,
	// Compilerに使用するProfile
	const wchar_t* profile,
	// 初期化で生成したものを3つ
	IDxcUtils* dxcUtils,
	IDxcCompiler3* dxcCompiler,
	IDxcIncludeHandler* includeHandler) {

#pragma region hlslファイルを読む

	// これからシェーダーをコンパイラする旨をログに出す
	WinApp::Log(ConvertString(std::format(L"Begin CompileShader, path:{}, profile:{}\n", filePath, profile)));
	// hlslファイルを読む
	IDxcBlobEncoding* shaderSource = nullptr;
	HRESULT hr = dxcUtils->LoadFile(filePath.c_str(), nullptr, &shaderSource);
	// 読めなかったら止める
	assert(SUCCEEDED(hr));
	// 読み込んだファイルの内容を設定する
	DxcBuffer shaderSourceBuffer;
	shaderSourceBuffer.Ptr = shaderSource->GetBufferPointer();
	shaderSourceBuffer.Size = shaderSource->GetBufferSize();
	shaderSourceBuffer.Encoding = DXC_CP_UTF8; // UTF8の文字コードであることを通知

#pragma endregion

#pragma region Compileする

	LPCWSTR arguments[] = {
		filePath.c_str(), // コンパイル対象のhlslファイル名
		L"-E", L"main", // エントリーポイントの指定。基本的にmain以外にはしない
		L"-T", profile, // ShaderProfileの設定
		L"-Zi", L"-Qembed_debug", // デバッグ用の情報を埋め込む
		L"-Od", // 最適化を外しておく
		L"-Zpr", // メモリレイアウトは行優先
	};
	// 実際にShaderをコンパイルする
	IDxcResult* shaderResult = nullptr;
	hr = dxcCompiler->Compile(
		&shaderSourceBuffer,	// 読み込んだファイル
		arguments,				// コンパイルオプション
		_countof(arguments),	// コンパイルオプションの数
		includeHandler,			// includeが含まれた諸々
		IID_PPV_ARGS(&shaderResult) // コンパイル結果
	);
	// コンパイルエラーではなくdxcが起動できないなど致命的な状況
	assert(SUCCEEDED(hr));

#pragma endregion

#pragma region 警告・エラーが出てないか確認する

	// 警告・エラーが出てきたらログに出して止める
	IDxcBlobUtf8* shaderError = nullptr;
	shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shaderError), nullptr);
	if (shaderError != nullptr && shaderError->GetStringLength() != 0) {
		WinApp::Log(shaderError->GetStringPointer());
		assert(false);
	}

#pragma endregion

#pragma region Compile結果を受け取って渡す

	// コンパイル結果から実行用のバイナリ部分を取得
	IDxcBlob* shaderBlob = nullptr;
	hr = shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob), nullptr);
	assert(SUCCEEDED(hr));
	// 成功したログを出す
	WinApp::Log(ConvertString(std::format(L"Compile Succeeded, path:{}, profile:{}\n", filePath, profile)));
	// もう使わないリソースを解放
	shaderSource->Release();
	shaderResult->Release();
	// 実行用のバイナリを変換
	return shaderBlob;

#pragma endregion

}

void MyEngine::CreateDescriptorRange() {
	descriptorRange_[0].BaseShaderRegister = 0;
	descriptorRange_[0].NumDescriptors = 1;
	descriptorRange_[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange_[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
}

void MyEngine::CraeteDescriptorTable() {
	rootParameters_[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameters_[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters_[2].DescriptorTable.pDescriptorRanges = descriptorRange_;
	rootParameters_[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange_);
}

void MyEngine::SettingSampler() {
	staticSamplers_[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	staticSamplers_[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers_[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers_[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers_[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	staticSamplers_[0].MaxLOD = D3D12_FLOAT32_MAX;
	staticSamplers_[0].ShaderRegister = 0;
	staticSamplers_[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	descriptionRootSignature_.pStaticSamplers = staticSamplers_;
	descriptionRootSignature_.NumStaticSamplers = _countof(staticSamplers_);
}

void MyEngine::CreateRootParameter() {
	rootParameters_[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters_[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters_[0].Descriptor.ShaderRegister = 0;
	rootParameters_[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters_[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	rootParameters_[1].Descriptor.ShaderRegister = 0;
	
	CraeteDescriptorTable();
	
	descriptionRootSignature_.pParameters = rootParameters_;
	descriptionRootSignature_.NumParameters = _countof(rootParameters_);
}

void MyEngine::CreateRootSignature() {
	HRESULT hr;

	descriptionRootSignature_.Flags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	// rootParameter生成
	CreateRootParameter();
	// samplerの設定
	SettingSampler();

	// シリアライズしてバイナリにする
	hr = D3D12SerializeRootSignature(&descriptionRootSignature_,
		D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob_, &errorBlob_);
	if (FAILED(hr)) {
		WinApp::Log(reinterpret_cast<char*>(errorBlob_->GetBufferPointer()));
		assert(false);
	}
	// バイナリをもとに生成
	hr = directXCommon_->GetDevice()->CreateRootSignature(0, signatureBlob_->GetBufferPointer(),
		signatureBlob_->GetBufferSize(), IID_PPV_ARGS(&rootSignature_));
	assert(SUCCEEDED(hr));
}

void MyEngine::SettingInputLayout() {
	inputElementDescs_[0].SemanticName = "POSITION";
	inputElementDescs_[0].SemanticIndex = 0;
	inputElementDescs_[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs_[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	
	inputElementDescs_[1].SemanticName = "TEXCOORD";
	inputElementDescs_[1].SemanticIndex = 0;
	inputElementDescs_[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputElementDescs_[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	inputLayoutDesc_.pInputElementDescs = inputElementDescs_;
	inputLayoutDesc_.NumElements = _countof(inputElementDescs_);
}

void MyEngine::SettingBlendState() {
	// すべての色要素を書き込む
	blendDesc_.RenderTarget[0].RenderTargetWriteMask =
		D3D12_COLOR_WRITE_ENABLE_ALL;
}

void MyEngine::SettingRasterizerState() {
	// 裏面(時計回り)を表示しない
	rasterizerDesc_.CullMode = D3D12_CULL_MODE_BACK;
	// 三角形の中を塗りつぶす
	rasterizerDesc_.FillMode = D3D12_FILL_MODE_SOLID;
}

void MyEngine::PixelSharder() {
	pixelShaderBlob_ = CompileShader(L"Object3d.PS.hlsl",
		L"ps_6_0", dxcUtils_, dxcCompiler_, includeHandler_);
	assert(pixelShaderBlob_ != nullptr);
}

void MyEngine::VertexSharder() {
	// Shaderをコンパイルする
	vertexShaderBlob_ = CompileShader(L"Object3d.VS.hlsl",
		L"vs_6_0", dxcUtils_, dxcCompiler_, includeHandler_);
	assert(vertexShaderBlob_ != nullptr);
}

void MyEngine::CreatePSO() {
	HRESULT hr;

	graphicsPipelineStateDescs_.pRootSignature = rootSignature_; // rootSignature
	graphicsPipelineStateDescs_.InputLayout = inputLayoutDesc_; // InputLayout
	graphicsPipelineStateDescs_.VS = { vertexShaderBlob_->GetBufferPointer(),
	vertexShaderBlob_->GetBufferSize() }; // vertexShader
	graphicsPipelineStateDescs_.PS = { pixelShaderBlob_->GetBufferPointer(),
	pixelShaderBlob_->GetBufferSize() }; // pixelShader
	graphicsPipelineStateDescs_.BlendState = blendDesc_; // blendState
	graphicsPipelineStateDescs_.RasterizerState = rasterizerDesc_; // rasterizerState
	// 書き込むRTVの情報
	graphicsPipelineStateDescs_.NumRenderTargets = 1;
	graphicsPipelineStateDescs_.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	// 利用するトポロ時（形状）のタイプ。三角形
	graphicsPipelineStateDescs_.PrimitiveTopologyType =
		D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	// どのように画面に色を打ち込むかの設定（気にしなくてよい）
	graphicsPipelineStateDescs_.SampleDesc.Count = 1;
	graphicsPipelineStateDescs_.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	// 実際に生成
	hr = directXCommon_->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDescs_,
		IID_PPV_ARGS(&graphicsPipelineState_));
	assert(SUCCEEDED(hr));
}

void MyEngine::PSO() {
	CreateDescriptorRange();

	CreateRootSignature();

	CreateRootParameter();

	SettingInputLayout();

	SettingBlendState();

	SettingRasterizerState();

	VertexSharder();

	PixelSharder();

	CreatePSO();
}

void MyEngine::CreateViewport() {
	// クライアント領域のサイズと一緒にして画面全体に表示
	viewport_.Width = (float)WinApp::kClientWidth_;
	viewport_.Height = (float)WinApp::kClientHeight_;
	viewport_.TopLeftX = 0;
	viewport_.TopLeftY = 0;
	viewport_.MinDepth = 0.0f;
	viewport_.MaxDepth = 1.0f;
}

void MyEngine::CreateScissor() {
	// 基本的にビューポートと同じ矩形が構成されるようにする
	scissorRect_.left = 0;
	scissorRect_.right = WinApp::kClientWidth_;
	scissorRect_.top = 0;
	scissorRect_.bottom = WinApp::kClientHeight_;
}

void MyEngine::VariableInitialize() {
	vertexLeft_[0].position = { -0.5f, -0.5f,0.0f,1.0f };
	vertexTop_[0].position = { 0.0f, 0.5f,0.0f,1.0f };
	vertexRight_[0].position = { 0.5f, -0.5f,0.0f,1.0f };

	//vertexLeft_[1] = { -0.2f, -0.8f,0.0f,1.0f };
	//vertexTop_[1] = { 0.0f, -0.6f,0.0f,1.0f };
	//vertexRight_[1] = { 0.2f, -0.8f,0.0f,1.0f };

	//vertexLeft_[2] = { -0.2f, -0.6f,0.0f,1.0f };
	//vertexTop_[2] = { 0.0f, -0.4f,0.0f,1.0f };
	//vertexRight_[2] = { 0.2f, -0.6f,0.0f,1.0f };

	//vertexLeft_[3] = { -0.2f, -0.4f,0.0f,1.0f };
	//vertexTop_[3] = { 0.0f, -0.2f,0.0f,1.0f };
	//vertexRight_[3] = { 0.2f, -0.4f,0.0f,1.0f };

	//vertexLeft_[4] = { -0.2f, -0.2f,0.0f,1.0f };
	//vertexTop_[4] = { 0.0f, 0.0f,0.0f,1.0f };
	//vertexRight_[4] = { 0.2f, -0.2f,0.0f,1.0f };

	//vertexLeft_[5] = { -0.2f, 0.0f,0.0f,1.0f };
	//vertexTop_[5] = { 0.0f, 0.2f,0.0f,1.0f };
	//vertexRight_[5] = { 0.2f, 0.0f,0.0f,1.0f };

	//vertexLeft_[6] = { -0.2f, 0.2f,0.0f,1.0f };
	//vertexTop_[6] = { 0.0f, 0.4f,0.0f,1.0f };
	//vertexRight_[6] = { 0.2f, 0.2f,0.0f,1.0f };

	//vertexLeft_[7] = { -0.2f, 0.4f,0.0f,1.0f };
	//vertexTop_[7] = { 0.0f, 0.6f,0.0f,1.0f };
	//vertexRight_[7] = { 0.2f, 0.4f,0.0f,1.0f };

	//vertexLeft_[8] = { -0.2f, 0.6f,0.0f,1.0f };
	//vertexTop_[8] = { 0.0f, 0.8f,0.0f,1.0f };
	//vertexRight_[8] = { 0.2f, 0.6f,0.0f,1.0f };

	//vertexLeft_[9] = { -0.2f, 0.8f,0.0f,1.0f };
	//vertexTop_[9] = { 0.0f, 1.0f,0.0f,1.0f };
	//vertexRight_[9] = { 0.2f, 0.8f,0.0f,1.0f };

	for (int i = 0; i < kMaxTriangle; i++) {
		Triangle_[i] = new Triangle();
		Triangle_[i]->Initialize(directXCommon_);
	}
}

void MyEngine::Initialize(const char* title, int32_t kClientWidth, int32_t kClientHeight) {
	// COMの初期化
	textureManager_.ComInit();

	// タイトルバーの変換
	auto&& titleString = ConvertString(title);
	WinApp::Initialize(titleString.c_str(), kClientWidth, kClientHeight);

	directXCommon_->Initialize();

	DXCInitialize();

	PSO();

	CreateViewport();

	CreateScissor();

	// Textureを読んで転送し、shaderResourceViewの生成
	textureManager_.TransferTexture(directXCommon_->GetDevice(), directXCommon_->GetCommandList(),directXCommon_->GetSrvDescriptorHeap());

	// 三角形の頂点データ
	VariableInitialize();

	// それぞれの三角形にtextureSrvHandleGPUをset
	for (int i = 0; i < kMaxTriangle; i++) {
		Triangle_[i]->SetTextureSrvHandleGPU(textureManager_.GetTextureSrvHandleGPU());
	}

	// カメラの初期化
	camera_.Initialize();
}

void MyEngine::BeginFrame() {
	// 描画前の処理
	directXCommon_->PreDraw();
	directXCommon_->GetCommandList()->RSSetViewports(1, &viewport_); // Viewportを設定
	directXCommon_->GetCommandList()->RSSetScissorRects(1, &scissorRect_); // Scirssorを設定
	// RootSignatureを設定。PSOに設定しているけど別途設定が必要
	directXCommon_->GetCommandList()->SetGraphicsRootSignature(rootSignature_);
	//directXCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(2, textureSrvHandleGPU_);
	directXCommon_->GetCommandList()->SetPipelineState(graphicsPipelineState_); // PSOを設定
	
	// カメラの設定
	camera_.SettingCamera();
}

void MyEngine::Draw() {
	for (int i = 0; i < kMaxTriangle; i++) {
		Triangle_[i]->Draw(vertexLeft_[i].position, vertexTop_[i].position, vertexRight_[i].position, Vector4{ 1.0f,1.0f,1.0f,1.0f },*camera_.GetTransformationMatrixData());
	}
}

void MyEngine::EndFrame() {
	// ImGui
	imGuiManager_->Draw();

	// 描画後の処理
	directXCommon_->PostDraw();
}

void MyEngine::Release() {
	directXCommon_->Release();
	for (int i = 0; i < kMaxTriangle; i++) {
		delete Triangle_[i];
	}
	graphicsPipelineState_->Release();
	signatureBlob_->Release();
	if (errorBlob_) {
		errorBlob_->Release();
	}
	rootSignature_->Release();
	pixelShaderBlob_->Release();
	vertexShaderBlob_->Release();
	textureManager_.Release();

	CloseWindow(WinApp::hwnd_);

	IDXGIDebug1* debug;
	// リソースリークチェック
	if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debug)))) {
		debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
		debug->ReportLiveObjects(DXGI_DEBUG_APP, DXGI_DEBUG_RLO_ALL);
		debug->ReportLiveObjects(DXGI_DEBUG_D3D12, DXGI_DEBUG_RLO_ALL);
		debug->Release();
	}
	// COMの終了処理
	textureManager_.ComUninit();
}