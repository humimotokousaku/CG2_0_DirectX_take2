#pragma once
#include "MyEngine.h"
#include "Triangle.h"

class GameScene
{
public:
	// ゲームループ
	void Run();

	// 初期化
	void Initialize();

	// 更新処理
	void Update();

	// 解放処理
	void Finalize();

private:
	WinApp* winApp_;
	DirectXCommon* directXCommon_;
	MyEngine* myEngine_;
	// 三角形を描画できる最大数
	static const int kMaxTriangle = 2;
	Triangle* triangle_[kMaxTriangle];
};

