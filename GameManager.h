#pragma once
#include "./base/DirectXCommon.h"
#include "scene/IScene.h"
#include "MyEngine.h"
#include "Triangle.h"
#include "./base/WinApp.h"

class GameManager{
public:
	// コンストラクタ
	GameManager();

	// ループ処理
	void Run();

	void ChangeState(IScene* pState);

	// 初期化
	void Initialize();

	// 解放処理
	void Finalize();

private:
	IScene* scene_;
	WinApp* winApp_;
	DirectXCommon* directXCommon_;
	MyEngine* myEngine_;
};

