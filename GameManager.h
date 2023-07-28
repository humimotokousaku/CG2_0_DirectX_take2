#pragma once
#include "Adapter.h"
#include "Triangle.h"
#include "Sphere.h"
#include "Sprite.h"
#include "MyEngine.h"

class GameManager
{	
public:

	// ループ処理
	void Run();

	//void ChangeState(IScene* pState);

	// 初期化
	void Initialize();

	// 解放処理
	void Finalize();
private:
	WinApp* winApp_;
	DirectXCommon* directXCommon_;
	// 三角形を描画できる最大数
	static const int kMaxTriangle = 2;
	Triangle* triangle_[kMaxTriangle];
	Sprite* sprite_;
	Sphere* sphere_;
	MyEngine* myEngine_;
	Light* light_;
	Camera* camera_;
	ImGuiManager* imGuiManager_;
	TextureManager* textureManager_;
	ObjModel* objModel_;
	//Adapter* adapter_;
};

