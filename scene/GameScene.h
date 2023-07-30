#pragma once
#include "IScene.h"
#include "../object/Triangle.h"

class GameManager;
class GameScene : public IScene
{
public:
	// 初期化
	void Initialize()override;

	// 更新処理
	void Update()override;

	// 描画
	void Draw()override;

	void Finalize()override;
private:
	// 三角形の描画する数
	static const int kMaxTriangle = 3;
	Triangle* triangle_[kMaxTriangle];
};

