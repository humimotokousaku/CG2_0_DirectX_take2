#pragma once
#include "IScene.h"
#include "../components/Input.h"
//#include "../Block.h"
#include "../Cube.h"

class GameManager;

class TitleScene : public IScene
{
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize()override;
private:
	//Block* block_;
	const static int kMaxCube = 200;
	Cube* cube_[kMaxCube];
	int textureNum_;
	Input* input_;
	Vector3 pos_;
	WorldTransform cubeWorldTransform_[kMaxCube];
	//WorldTransform worldTransform_;
	ViewProjection viewProjection_;
};