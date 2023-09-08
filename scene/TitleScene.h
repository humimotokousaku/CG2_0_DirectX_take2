#pragma once
#include "IScene.h"
#include "../components/Input.h"
#include "../Block.h"

class GameManager;

class TitleScene : public IScene
{
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize()override;
private:
	Block* block_;
	int textureNum_;
	Input* input_;
	Vector3 pos_;

	WorldTransform worldTransform_;
	ViewProjection viewProjection_;
};