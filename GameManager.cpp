#include "scene/ClearScene.h"
#include "GameManager.h"
#include "scene/GameScene.h"
#include "scene/TitleScene.h"

GameManager::GameManager() {
	scene_ = new TitleScene();
}

void GameManager::ChangeState(IScene* pState) {
	delete scene_;
	scene_ = pState;
	// シーンが変わったら初期化
	scene_->Initialize(this);
}

void GameManager::Initialize() {
	// windowの初期化
	winApp_ = WinApp::GetInstance();
	winApp_->Initialize();

	// DirectXの初期化
	directXCommon_ = DirectXCommon::GetInstance();
	directXCommon_->Initialize();

	// エンジンの初期化
	myEngine_ = new MyEngine();
	myEngine_->Initialize();

	// シーンごとの初期化
	scene_->Initialize(this);
}

void GameManager::Run() {
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
			// 描画前の処理
			myEngine_->BeginFrame();

			// シーンごとの更新処理(三角形を描画するだけなので今は処理なし)
			scene_->Update(this);
			// シーンごとの描画
			scene_->Draw(this);

			// 描画後の処理
			myEngine_->EndFrame();		
		}
	}
	// 解放処理
	Finalize();
}

void GameManager::Finalize() {
	scene_->Finalize(this);
	delete scene_;
	myEngine_->Finalize();
	delete myEngine_;
	directXCommon_->Finalize();

	IDXGIDebug1* debug;
	// リソースリークチェック
	if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debug)))) {
		debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
		debug->ReportLiveObjects(DXGI_DEBUG_APP, DXGI_DEBUG_RLO_ALL);
		debug->ReportLiveObjects(DXGI_DEBUG_D3D12, DXGI_DEBUG_RLO_ALL);
		debug->Release();
	}
}