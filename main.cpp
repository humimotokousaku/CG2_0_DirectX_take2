#include "MyEngine.h"

const char kWindowTitle[] = "CG2_CLASS";

struct D3DResourceLeakCheker {
	~D3DResourceLeakCheker() {
		// リリースリークチェック
		Microsoft::WRL::ComPtr<IDXGIDebug1> debug;
		if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debug)))) {
			debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
			debug->ReportLiveObjects(DXGI_DEBUG_APP, DXGI_DEBUG_RLO_ALL);
			debug->ReportLiveObjects(DXGI_DEBUG_D3D12, DXGI_DEBUG_RLO_ALL);
		}
	}
};

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	D3DResourceLeakCheker leakCheck;
	std::unique_ptr<MyEngine> engine;
	engine = std::make_unique<MyEngine>();
	// エンジンの初期化
	engine->Initialize(kWindowTitle, 1280, 720);

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
	engine->Finalize();
	
#pragma endregion

	return 0;
}