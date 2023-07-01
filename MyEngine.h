#pragma once
#include "WinApp.h"
#include "DirectXCommon.h"
#include "Triangle.h"
#include "ImGuiManager.h"
#include "TextureManager.h"

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

	// DescriptorRangeの生成
	void CreateDescriptorRange();

	// DescriptorTableの生成
	void CraeteDescriptorTable();

	// Samplerの設定
	void SettingSampler();

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

	// heapの設定
	void SettingHeapProperties();

	// エンジンの初期化
	void Initialize(const char* title, int32_t kClientWidth, int32_t kClientHeight);

	// 描画前の処理
	void BeginFrame();

	// 三角形描画
	void Draw();

	// 描画後の処理
	void EndFrame();

	// 解放処理
	void Release();

private:
	// 三角形を描画できる最大数
	static const int kMaxTriangle = 2;
	Triangle* Triangle_[kMaxTriangle];
	DirectXCommon* directXCommon_;
	IDxcUtils* dxcUtils_;
	IDxcCompiler3* dxcCompiler_;
	IDxcIncludeHandler* includeHandler_;
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature_;
	ID3DBlob* signatureBlob_;
	ID3DBlob* errorBlob_;
	ID3D12RootSignature* rootSignature_;
	D3D12_INPUT_ELEMENT_DESC inputElementDescs_[2];
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc_;
	D3D12_BLEND_DESC blendDesc_;
	D3D12_RASTERIZER_DESC rasterizerDesc_;
	IDxcBlob* vertexShaderBlob_;
	IDxcBlob* pixelShaderBlob_;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDescs_;
	ID3D12PipelineState* graphicsPipelineState_;
	D3D12_VIEWPORT viewport_;
	D3D12_RECT scissorRect_;

	VertexData vertexLeft_[kMaxTriangle];
	VertexData vertexTop_[kMaxTriangle];
	VertexData vertexRight_[kMaxTriangle];

	D3D12_ROOT_PARAMETER rootParameters_[3];
	D3D12_DESCRIPTOR_RANGE descriptorRange_[1];
	D3D12_STATIC_SAMPLER_DESC staticSamplers_[1];

	Camera camera_;
	ImGuiManager* imGuiManager_;
	TextureManager textureManager_;
	DirectX::ScratchImage mipImages_;
	ID3D12Resource* textureResource_;
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc_;
	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU_;
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU_;
	ID3D12Resource* intermediateResource_;
	D3D12_HEAP_PROPERTIES heapProperties_;
};
