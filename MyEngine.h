#pragma once
#include "Triangle.h"
#include "WinApp.h"
#include "DirIectXCommon.h"

class MyEngine {
public:
	// DXCの初期化
	void DXCInitialize();

	// シェーダのコンパイル
	IDxcBlob* CompileShader(
		// CompilerするShaderファイルへのパス
		const std::wstring& filePath,
		// Compilerに使用するProfile
		const wchar_t* profile,
		// 初期化で生成したものを3つ
		IDxcUtils* dxcUtils,
		IDxcCompiler3* dxcCompiler,
		IDxcIncludeHandler* includeHandler);

	// RootSignatureの生成
	void CreateRootSignature();

	// RootParameter生成
	void CreateRootParameter();

	// InputLayerの設定
	void SettingInputLayout();

	// BlendStateの設定
	void SettingBlendState();

	// RasterizerStateの設定
	void SettingRasterizerState();

	// ピクセルシェーダー
	void PixelSharder();

	// 頂点シェーダー
	void VertexSharder();

	// PSOの生成
	void CreatePSO();

	// PSO
	void PSO();

	// ビューポート
	void CreateViewport();

	// シザー矩形
	void CreateScissor();

	// 頂点データの初期化
	void VariableInitialize();

	// エンジンの初期化
	void Initialize();

	// 描画前の処理
	void BeginFrame();

	// 三角形描画
	void Draw();

	// 描画後の処理
	void EndFrame();

	// 解放処理
	void Release();

public:
	// 三角形を描画できる最大数
	static const int kMaxTriangle = 10;
	Triangle* Triangle_[kMaxTriangle];
	static	DirectXCommon* directXCommon_;
	IDxcUtils* dxcUtils;
	IDxcCompiler3* dxcCompiler;
	IDxcIncludeHandler* includeHandler;
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature;
	ID3DBlob* signatureBlob;
	ID3DBlob* errorBlob;
	ID3D12RootSignature* rootSignature;
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[1];
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc;
	D3D12_BLEND_DESC blendDesc;
	D3D12_RASTERIZER_DESC rasterizerDesc;
	IDxcBlob* vertexShaderBlob;
	IDxcBlob* pixelShaderBlob;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDescs;
	ID3D12PipelineState* graphicsPipelineState;
	D3D12_VIEWPORT viewport;
	D3D12_RECT scissorRect;

	Vector4 vertexLeft[kMaxTriangle];
	Vector4 vertexTop[kMaxTriangle];
	Vector4 vertexRight[kMaxTriangle];

	D3D12_ROOT_PARAMETER rootParameters[1];
};

