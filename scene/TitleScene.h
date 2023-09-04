#pragma once
#include "IScene.h"
#include "../object/Triangle.h"
#include "../object/Sprite.h"
#include "../object/Sphere.h"
#include "../object/Plane.h"
#include "../object/Axis.h"
#include "../object/MultiMesh.h"
#include "../object/MultiMaterial.h"
#include "../object/Bunny.h"

class GameManager;

class TitleScene : public IScene
{
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize()override;
private:
	// 三角形を描画できる最大数
	static const int kMaxTriangle = 2;
	Triangle* triangle_[kMaxTriangle];
	Sphere* sphere_;
	Sprite* sprite_;
	Plane* plane_;
	Axis* axis_;
	MultiMesh* multiMesh_;
	MultiMaterial* multiMaterial_;
	Bunny* bunny_;
};