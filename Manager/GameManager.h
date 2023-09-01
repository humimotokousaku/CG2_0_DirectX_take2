#pragma once

#pragma region include
 
// Scene
#include "../scene/IScene.h"
#include "../scene/GameScene.h"
#include "../scene/TitleScene.h"

// Manager
#include "../Manager/PipelineManager.h"
#include "ObjManager.h"

// Base
#include "../base/WinApp.h"
#include "../base/DirectXCommon.h"

// components
#include "../light/Light.h"
#include "../camera/Camera.h"
#include "../DebugCamera.h"
#include "../Input.h"
#include "../Audio.h"

#pragma endregion

class GameManager
{	
public:
	///
	/// Default Method
	/// 
	// コンストラクタ
	GameManager();

	// デストラクタ
	//~GameManager();

	// 初期化
	void Initialize();

	// 更新処理
	//void Update();

	// 解放処理
	void Finalize();

	///
	/// user method
	///
	// ループ処理
	void Run();

	// 描画前の処理
	void BeginFrame();

	// 描画後の処理
	void EndFrame();

	// ImGuiのパラメータを入れる
	void ImGuiAdjustParameter();

private:
	// base
	WinApp* winApp_;
	DirectXCommon* directXCommon_;
	MyEngine* myEngine_;

	// components
	Camera* camera_;
	DebugCamera* debugCamera_;
	Light* light_;
	ObjManager* objManager_;
	TextureManager* textureManager_;
	ImGuiManager* imGuiManager_;
	Input* input_;
	Audio* audio_;
	Microsoft::WRL::ComPtr<IXAudio2> xAudio2_;
	IXAudio2MasteringVoice* masterVoice_;
	SoundData soundData1_;

	// scene
	IScene* sceneArr_[2];
	int sceneNum_;
	int preSceneNum_;
};

