#include "MyEngine.h"

const char kWindowTitle[] = "CG2_CLASS";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	//CoInitializeEx(0, COINIT_MULTITHREADED);
	MyEngine* engine = new MyEngine();
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