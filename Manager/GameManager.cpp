#include "GameManager.h"
#include "../utility/ConvertString.h"
#include "../GlobalVariables.h"

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
	winApp_ = WinApp::GetInstance();
	winApp_->Initialize(titleString.c_str(), 1280, 720);

	// DirectXの初期化
	directXCommon_ = DirectXCommon::GetInstance();
	directXCommon_->DirectXCommon::GetInstance()->Initialize(winApp_->GetHwnd());

	input_ = Input::GetInstance();
	input_->Initialize();

	// Audioの初期化
	audio_ = Audio::GetInstance();
	//HRESULT result;
	// Xaudio2エンジンのインスタンスを生成
//	result = XAudio2Create(&xAudio2_, 0, XAUDIO2_DEFAULT_PROCESSOR);
	// マスターボイスを生成
//	result = xAudio2_->CreateMasteringVoice(&masterVoice_);
	// 音声読み込み
//	soundData1_ = audio_->SoundLoadWave("resources/fanfare.wav");
	// 音声再生
	//audio_->SoundPlayWave(xAudio2_.Get(), soundData1_);

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
	// デバッグカメラの初期化
	debugCamera_ = DebugCamera::GetInstance();
	debugCamera_->initialize();

	// カメラの初期化
	camera_ = Camera::GetInstance();
	camera_->Initialize();

	// ImGuiの初期化
	imGuiManager_ = new ImGuiManager();
	imGuiManager_->Initialize(winApp_->GetHwnd());

	// ブローバル変数の読み込み
	GlobalVariables::GetInstance()->LoadFiles();

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
			//input_->CheckAllKeyStates();

			if (input_->ReleaseKey(DIK_0)) {
				WinApp::Log("Hit 0\n");
			}

			//
			//if (Input::GetInstance()->GetJoystickState(0, joyState)) {
			//	// 移動量
			//	Vector3 move{
			//		(float)joyState.Gamepad.sThumbLX / SHRT_MAX, 
			//		(float)joyState.Gamepad.sThumbLY / SHRT_MAX,
			//		0
			//	};
			//	//ImGui::Begin("test joyStick");
			//	//ImGui::Text("joystick %f, %f, %f", move.x, move.y, move.z);
			//	//ImGui::End();
			//}

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
	xAudio2_.Reset();
	audio_->SoundUnload(&soundData1_);
	// Textureのゲーム終了処理
	textureManager_->ComUninit();
}

void GameManager::BeginFrame() {
	input_->Update();
	myEngine_->BeginFrame();
	// デバッグカメラ
	debugCamera_->Update();
	// カメラの設定
	camera_->SettingCamera();

	// ImGui
	imGuiManager_->PreDraw();
	// グローバル変数の更新
	GlobalVariables::GetInstance()->Update();
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
		if (ImGui::BeginTabItem("Half Lambert")) {
			light_->ImGuiAdjustParameter();
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
	ImGui::End();
}