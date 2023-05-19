#include <Windows.h>
#include "ConvertString.h"
#include <cstdint>
#include <format>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#include <dxgidebug.h>
#pragma comment(lib, "dxguid.lib")
#include <dxcapi.h>
#pragma comment(lib, "dxcompiler.lib")
#include "WinApp.h"
#include "DirIectXCommon.h"

struct Vector4 final {
	float x;
	float y;
	float z;
	float w;
};

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	// 出力ウィンドウへの文字出力
	OutputDebugStringA("Hello,DirectX!\n");

	// windowsの初期化
	WinApp windows;
	windows.Initialize();

	// directX初期化のためのインスタンス
	DirectXCommon* directX = new DirectXCommon();

	// DebugLayer
	directX->DebugLayer();

	// ウィンドウを表示する
	ShowWindow(windows.hwnd_, SW_SHOW);

	// DirectX初期化
	directX->Initialize();

#pragma region メインループ

	MSG msg{};
	// ウィンドウの×ボタンが押されるまでループ
	while (msg.message != WM_QUIT) {
		// Windowにメッセージが来ていたら最優先で処理させる
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			// ループ処理

			// スクリーンの色を変える
			directX->WindowChangeColor();
		}
	}
	// 解放処理
	directX->Release();

#pragma endregion

	return 0;
}