#pragma once
#include "MyEngine.h"
#include "Triangle.h"

class GameScene
{
public:
	void Initialize();
	void Update();
	void Run();
	void Finalize();

private:
	WinApp* winApp_;
	DirectXCommon* directXCommon_;
	// 三角形を描画できる最大数
	static const int kMaxTriangle = 2;
	MyEngine* myEngine_;
	Triangle* triangle_[kMaxTriangle];
};

