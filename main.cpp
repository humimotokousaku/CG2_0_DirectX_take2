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

#include "MyEngine.h"
#include "Matrix4x4.h"
#include "Transform.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	MyEngine* engine = new MyEngine();
	// エンジンの初期化
	engine->Initialize();

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
			engine->BeginFrame();

			// 三角形
			engine->Draw();

			engine->EndFrame();
		}
	}
	// 解放処理
	engine->Release();

#pragma endregion

	return 0;
}