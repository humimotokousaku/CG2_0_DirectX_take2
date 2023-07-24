#include "GameScene.h"

void GameScene::Initialize() {
	// windowの初期化
	winApp_ = new WinApp();
	winApp_->Initialize();

	// DirectXの初期化
	directXCommon_ = new DirectXCommon();
	directXCommon_->Initialize();

	// エンジンの初期化
	myEngine_ = new MyEngine();
	myEngine_->Initialize(directXCommon_);

	// 三角形の初期化
	for (int i = 0; i < kMaxTriangle; i++) {
		triangle_[i] = new Triangle({ -0.2f, -0.2f * (i+1), 0.0f, 1.0f }, { 0.0f, -0.1f * (i + 1), 0.0f, 1.0f }, { 0.2f, -0.2f * (i + 1), 0.0f, 1.0f });
		triangle_[i]->Initialize(directXCommon_);
	}
}

void GameScene::Update() {
	// 描画前の処理
	myEngine_->BeginFrame();

	// 三角形を描画
	for (int i = 0; i < kMaxTriangle; i++) {
		triangle_[i]->Draw();
	}

	// 描画後の処理
	myEngine_->EndFrame();
}

void GameScene::Run() {
	// 初期化
	Initialize();
	MSG msg{};
	// ウィンドウの×ボタンが押されるまでループ
	while (msg.message != WM_QUIT) {
		// Windowにメッセージが来ていたら最優先で処理させる
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			Update();
		}
	}
	Finalize();
}

void GameScene::Finalize() {
	for (int i = 0; i < kMaxTriangle; i++) {
		delete triangle_[i];
	}
	myEngine_->Release();
	delete myEngine_;
	directXCommon_->Release();
}

