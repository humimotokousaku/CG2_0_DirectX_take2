#pragma once
#include "../scene/IScene.h"
#include "../scene/GameScene.h"
#include "../scene/TitleScene.h"
#include "../Manager/PipelineManager.h"
#include "ObjManager.h"
#include "../base/WinApp.h"
#include "../base/DirectXCommon.h"
#include "../light/Light.h"
#include "../camera/Camera.h"
#include "../DebugCamera.h"
#include "../Audio.h"
#include "../Input.h"

class GameManager
{	
public:
	GameManager();

	// 初期化
	void Initialize();

	// 解放処理
	void Finalize();

	// ループ処理
	void Run();

	// 描画前の処理
	void BeginFrame();

	// 描画後の処理
	void EndFrame();

	// ImGuiのパラメータを入れる
	void ImGuiAdjustParameter();

private:
	WinApp* winApp_;
	DirectXCommon* directXCommon_;
	MyEngine* myEngine_;
	Light* light_;
	Camera* camera_;
	DebugCamera* debugCamera_;
	ImGuiManager* imGuiManager_;
	ObjManager* objManager_;
	TextureManager* textureManager_;
	IScene* sceneArr_[2];
	int sceneNum_;
	int preSceneNum_;
	Audio* audio_;
	Microsoft::WRL::ComPtr<IXAudio2> xAudio2_;
	IXAudio2MasteringVoice* masterVoice_;
	SoundData soundData1_;
	Input* input_;
};

