#pragma once
#include "IScene.h"
#include "../components/Input.h"
#include "../object/Sprite.h"
#include "../object/Sphere.h"

class GameManager;

class TitleScene : public IScene
{
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize()override;
private:
	Sprite* sprite_;
	Sphere* sphere_;
	int textureNum_;
	Input* input_;
	Vector3 pos_;
};