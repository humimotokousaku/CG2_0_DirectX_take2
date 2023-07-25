#pragma once
#include "IScene.h"
#include "../Triangle.h"

class GameManager;
class ClearScene : public IScene
{
public:
	void Initialize(GameManager* gameManager) override;
	void Update(GameManager* gameManager) override;
	void Draw(GameManager* gameManager) override;
	void Finalize(GameManager* gameManager) override;
private:
	// 三角形を描画できる最大数
	static const int kMaxTriangle = 4;
	Triangle* triangle_[kMaxTriangle];
};

