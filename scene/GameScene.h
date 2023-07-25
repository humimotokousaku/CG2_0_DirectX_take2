#pragma once
#include "IScene.h"
#include "../MyEngine.h"
#include "../Triangle.h"

class GameManager;
class GameScene : public IScene
{
public:
	// 初期化
	void Initialize(GameManager* gameManager)override;

	// 更新処理
	void Update(GameManager* gameManager)override;

	// 描画
	void Draw(GameManager* gameManager)override;

	void Finalize(GameManager* gameManager)override;
private:
	// 三角形の描画する数
	static const int kMaxTriangle = 3;
	Triangle* triangle_[kMaxTriangle];
};

