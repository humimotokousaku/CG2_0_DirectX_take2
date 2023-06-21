#include "MyEngine.h"

const char kWindowTitle[] = "CG2_CLASS";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	// engineの実体を生成
	MyEngine* engine = new MyEngine();
	// エンジンの初期化
	engine->Initialize(kWindowTitle, 1280, 720);

	MSG msg{};
	// ウィンドウの×ボタンが押されるまでループ
	while (msg.message != WM_QUIT) {
		// Windowにメッセージが来ていたら最優先で処理させる
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {// ループ処理
			// 描画前の処理
			engine->BeginFrame();

			// 三角形
			engine->Draw();

			// 描画後の処理
			engine->EndFrame();
		}
	}
	// 解放処理
	engine->Release();

	return 0;
}