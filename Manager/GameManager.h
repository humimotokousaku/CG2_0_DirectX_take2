#pragma once
#include "../scene/IScene.h"
#include "../scene/GameScene.h"
#include "../scene/TitleScene.h"
#include "../object/Triangle.h"
#include "../object/Sphere.h"
#include "../object/Sprite.h"
#include "../object/Plane.h"
#include "../object/Axis.h"
#include "../Manager/PipelineManager.h"
#include "ObjManager.h"
#include "../base/WinApp.h"
#include "../base/DirectXCommon.h"
#include "../light/Light.h"
#include "../camera/Camera.h"

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

	// シーンの切り替え
	void ChangeState(IScene* pState);

	// ImGuiのパラメータを入れる
	void ImGuiAdjustParameter();

private:
	WinApp* winApp_;
	DirectXCommon* directXCommon_;
	IScene* scene_;
	MyEngine* myEngine_;
	Light* light_;
	Camera* camera_;
	ImGuiManager* imGuiManager_;
	ObjManager* objManager_;
	TextureManager* textureManager_;
};

