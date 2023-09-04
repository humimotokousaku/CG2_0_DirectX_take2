#pragma once
#include "IScene.h"
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
};