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

}

void TitleScene::Draw() {
	sprite_->Draw(pos_, textureNum_);
	sphere_->Draw();
}

void TitleScene::Finalize() {
	delete sprite_;
	delete sphere_;
}