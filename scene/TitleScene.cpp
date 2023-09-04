#include "TitleScene.h"
#include "../Manager/GameManager.h"

void TitleScene::Initialize() {
	for (int i = 0; i < kMaxTriangle; i++) {
		triangle_[i] = new Triangle({ -1.0f, -0.2f * (i + 1), 0.0f, 1.0f }, { 0.0f, 0.2f * (i + 1), 0.0f, 1.0f }, { 1.0f, -0.2f * (i + 1), 0.0f, 1.0f });
		triangle_[i]->Initialize();
	}
	sprite_ = new Sprite();
	sprite_->Initialize();
	sphere_ = new Sphere();
	sphere_->Initialize();
	plane_ = new Plane();
	plane_->Initialize();
	axis_ = new Axis();
	axis_->Initialize();
	multiMesh_ = new MultiMesh();
	multiMesh_->Initialize();
	multiMaterial_ = new MultiMaterial();
	multiMaterial_->Initialize();
	bunny_ = new Bunny();
	bunny_->Initialize();
}

void TitleScene::Update() {

}

void TitleScene::Draw() {
	for (int i = 0; i < kMaxTriangle; i++) {
		triangle_[i]->Draw();
	}
	sphere_->Draw();
	sprite_->Draw();
	plane_->Draw();
	axis_->Draw();
	multiMesh_->Draw();
	multiMaterial_->Draw();
	bunny_->Draw();
}

void TitleScene::Finalize() {
	for (int i = 0; i < kMaxTriangle; i++) {
		delete triangle_[i];
	}
	delete sphere_;
	delete sprite_;
	delete plane_;
	delete axis_;
	delete multiMesh_;
	delete multiMaterial_;
	delete bunny_;
}