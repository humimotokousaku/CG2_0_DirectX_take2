#pragma once
#include "../object/Triangle.h"
#include "IScene.h"
#include "../object/Sprite.h"
#include "../object/Sphere.h"
#include "../object/Plane.h"
#include "../object/Axis.h"

class GameManager;

class TitleScene : public IScene
{
public:
	void Initialize(GameManager* gameManager) override;
	void Update(GameManager* gameManager) override;
	void Draw(GameManager* gameManager) override;
	void Finalize(GameManager* gameManager)override;
private:
	// 三角形を描画できる最大数
	static const int kMaxTriangle = 2;
	Triangle* triangle_[kMaxTriangle];
	Sphere* sphere_;
	Sprite* sprite_;
	Plane* plane_;
	Axis* axis_;
};