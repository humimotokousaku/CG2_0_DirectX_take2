#include "TitleScene.h"
#include "../Manager/GameManager.h"

void TitleScene::Initialize() {
	sprite_ = new Sprite();
	sprite_->Initialize();
	sphere_ = new Sphere();
	sphere_->Initialize();
	textureNum_ = UVCHEKER;
	input_ = Input::GetInstance();
}

void TitleScene::Update() {
	if (input_->TriggerKey(DIK_1)) {
		textureNum_ = MONSTERBALL;
	}
	else if (input_->TriggerKey(DIK_0)) {
		textureNum_ = UVCHEKER;
	}
}

void TitleScene::Draw() {
	sprite_->Draw(textureNum_);
	sphere_->Draw();
}

void TitleScene::Finalize() {
	delete sprite_;
	delete sphere_;
}