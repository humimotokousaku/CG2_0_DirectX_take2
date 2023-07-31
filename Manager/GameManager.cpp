#include "GameManager.h"
#include "../utility/ConvertString.h"

GameManager::GameManager() {
	 // 各シーンの配列
	sceneArr_[TITLESCENE] =new TitleScene();
	sceneArr_[GAMESCENE] = new GameScene();
}

void GameManager::Initialize() {

	const char kWindowTitle[] = "CG2_CLASS";
	// タイトルバーの変換
	auto&& titleString = ConvertString(kWindowTitle);

	// windowの初期化
	winApp_ = new WinApp();
	winApp_->Initialize(titleString.c_str(), 1280, 720);

	// DirectXの初期化
	directXCommon_ = DirectXCommon::GetInstance();
	directXCommon_->DirectXCommon::GetInstance()->Initialize(winApp_->GetHwnd());

	// objManagerの初期化。今はobjファイルの読み込みだけしている
	objManager_ = ObjManager::GetInstance();
	objManager_->Initialize();

	// Textureの初期化
	textureManager_ = TextureManager::GetInstance();
	textureManager_->TextureManager::GetInstance()->Initialize();
	// エンジンの初期化
	myEngine_ = new MyEngine();
	myEngine_->Initialize();

	// ライトの設定
	light_ = Light::GetInstance();
	light_->Initialize(DirectXCommon::GetInstance()->GetDevice());

	// カメラの初期化
	camera_ = Camera::GetInstance();
	camera_->Initialize();

	// ImGuiの初期化
	imGuiManager_ = new ImGuiManager();
	imGuiManager_->Initialize(winApp_->GetHwnd());

	//初期シーンの設定
	sceneNum_ = TITLESCENE;

	// シーンごとの初期化
	sceneArr_[sceneNum_]->Initialize();
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
			BeginFrame();

			// シーンチェック
			preSceneNum_ = sceneNum_;
			sceneNum_ = sceneArr_[sceneNum_]->GetSceneNum();

			//シーン変更チェック
			if (sceneNum_ != preSceneNum_) {
				sceneArr_[sceneNum_]->Initialize();
				sceneArr_[preSceneNum_]->Finalize();
			}

			// シーンごとの更新処理
			sceneArr_[sceneNum_]->Update(); 

			// ImGuiのパラメータを入れている
			ImGuiAdjustParameter();

			// シーンごとの描画処理
			sceneArr_[sceneNum_]->Draw();

			// 描画後の処理
			EndFrame();
		}
	}
	// 解放処理
	Finalize();
}

void GameManager::Finalize() {
	sceneArr_[sceneNum_]->Finalize();
	for (int i = 0; i < 2; i++) {	
		delete sceneArr_[i];
	}
	// ImGui
	imGuiManager_->Release();
	delete imGuiManager_;
	delete myEngine_;
	textureManager_->Release();
	directXCommon_->Release();
	CloseWindow(winApp_->GetHwnd());
	// Textureのゲーム終了処理
	textureManager_->ComUninit();
}

void GameManager::BeginFrame() {
	myEngine_->BeginFrame();
	// カメラの設定
	camera_->SettingCamera();
	// ImGui
	imGuiManager_->PreDraw();
}

void GameManager::EndFrame() {
	// ImGui
	imGuiManager_->PostDraw();

	myEngine_->EndFrame();
}

void GameManager::ImGuiAdjustParameter() {
	ImGui::Begin("CommonSettings");
	if (ImGui::BeginTabBar("CommonTabBar"))
	{
		// カメラのImGui
		if (ImGui::BeginTabItem("Camera")) {
			camera_->Camera::GetInstance()->DrawDebugParameter();
			ImGui::EndTabItem();
		}
		// ライトのImGui
		if (ImGui::BeginTabItem("Light")) {
			light_->ImGuiAdjustParameter();
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
	ImGui::End();
}