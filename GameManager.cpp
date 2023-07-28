#include "GameManager.h"
#include "ConvertString.h"

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

	objModel_ = new ObjModel();
	objModel_->Initialize(DirectXCommon::GetInstance()->GetDevice(), DirectXCommon::GetInstance()->GetCommandList());
	// Textureの初期化
	textureManager_ = TextureManager::GetInstance();
	textureManager_->Initialize(directXCommon_->GetDevice(), DirectXCommon::GetInstance()->GetCommandList(), DirectXCommon::GetInstance()->GetSrvDescriptorHeap(), objModel_->GetModelData());
	// エンジンの初期化
	myEngine_ = new MyEngine();
	myEngine_->Initialize(kWindowTitle, 1280, 720, directXCommon_);
	sprite_ = new Sprite();
	sprite_->Initialize(DirectXCommon::GetInstance()->GetDevice(), DirectXCommon::GetInstance()->GetCommandList());
	sphere_ = new Sphere();
	sphere_->Initialize(DirectXCommon::GetInstance()->GetDevice(), DirectXCommon::GetInstance()->GetCommandList());

	// 三角形の生成
	for (int i = 0; i < kMaxTriangle; i++) {
		triangle_[i] = new Triangle({ -1.0f, -0.2f * (i + 1), 0.0f, 1.0f }, { 0.0f, 0.1f * (i + 1), 0.0f, 1.0f }, { 1.0f, -0.2f * (i + 1), 0.0f, 1.0f });
		// 初期化
		triangle_[i]->Initialize();
		// 使う画像
		triangle_[i]->SetTextureSrvHandleGPU(*textureManager_->GetTextureSrvHandleGPU());
	}
	// ライトの設定
	light_ = new Light();
	light_->Initialize(DirectXCommon::GetInstance()->GetDevice());

	// カメラの初期化
	camera_ = new Camera();
	camera_->Initialize();

	// ImGuiの初期化
	imGuiManager_ = new ImGuiManager();
	imGuiManager_->Initialize(DirectXCommon::GetInstance()->GetDevice(), DirectXCommon::GetInstance()->GetSwapChainDesc(), DirectXCommon::GetInstance()->GetRtvDesc(), DirectXCommon::GetInstance()->GetSrvDescriptorHeap(), winApp_->GetHwnd());

	// シーンごとの初期化
	//scene_->Initialize(this);
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
			// カメラの設定
			camera_->SettingCamera();
			// ImGui
			imGuiManager_->PreDraw();

			Vector4 color = { 1.0f,1.0f,1.0f,1.0f };
			// 三角形
			for (int i = 0; i < kMaxTriangle; i++) {
				triangle_[i]->Draw(color, *camera_->GetTransformationMatrixData());
			}
			// 球
			sphere_->Draw(DirectXCommon::GetInstance()->GetDevice(), DirectXCommon::GetInstance()->GetCommandList(), textureManager_->GetTextureSrvHandleGPU(), *camera_->GetTransformationMatrixData(), light_->GetDirectionalLightResource().Get());
			// スプライト
			objModel_->Draw(DirectXCommon::GetInstance()->GetDevice(), DirectXCommon::GetInstance()->GetCommandList(), textureManager_->GetTextureSrvHandleGPU(), *camera_->GetTransformationMatrixData(), light_->GetDirectionalLightResource().Get());
			// sprite
			sprite_->Draw(DirectXCommon::GetInstance()->GetDevice(), DirectXCommon::GetInstance()->GetCommandList(), textureManager_->GetTextureSrvHandleGPU(), light_->GetDirectionalLightResource().Get());

			// カメラのImGui
			camera_->DrawDebugParameter();
			// ライトのImGui
			light_->DrawDebugParameter();

			// シーンごとの更新処理(三角形を描画するだけなので今は処理なし)
			//scene_->Update(this);
			// シーンごとの描画
			//scene_->Draw(this);

			// ImGui
			imGuiManager_->PostDraw(DirectXCommon::GetInstance()->GetCommandList());
			// 描画後の処理
			myEngine_->EndFrame();
		}
	}
	// 解放処理
	Finalize();
}

void GameManager::Finalize() {
	//scene_->Finalize(this);
	//delete scene_;
	for (int i = 0; i < kMaxTriangle; i++) {

		delete triangle_[i];
	}
	sphere_->Release();
	delete sphere_;
	sprite_->Release();
	delete sprite_;
	// ImGui
	imGuiManager_->Release();
	delete imGuiManager_;
	myEngine_->Finalize();
	delete myEngine_;
	light_->Release();
	delete light_;
	delete camera_;
	textureManager_->Release();
	objModel_->Release();
	delete objModel_;
	directXCommon_->Release();
	CloseWindow(winApp_->GetHwnd());
	// Textureのゲーム終了処理
	textureManager_->ComUninit();

	//IDXGIDebug1* debug;
	//// リソースリークチェック
	//if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debug)))) {
	//	debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
	//	debug->ReportLiveObjects(DXGI_DEBUG_APP, DXGI_DEBUG_RLO_ALL);
	//	debug->ReportLiveObjects(DXGI_DEBUG_D3D12, DXGI_DEBUG_RLO_ALL);
	//	debug->Release();
	//}
}