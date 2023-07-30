#include "TitleScene.h"
#include "GameManager.h"

void TitleScene::Initialize(GameManager* gameManager) {
	// 三角形
	for (int i = 0; i < kMaxTriangle; i++) {
		triangle_[i] = new Triangle({ -1.0f, -0.4f * (i + 1), 0.0f, 1.0f }, { 0.0f, -0.1f * (i + 1), 0.0f, 1.0f }, { 1.0f, -0.4f * (i + 1), 0.0f, 1.0f });
		triangle_[i]->Initialize();
	}
	// スプライト
	sprite_ = new Sprite();
	sprite_->Initialize();
	// 球体
	sphere_ = new Sphere();
	sphere_->Initialize();
	// 平面
	plane_ = new Plane();
	plane_->Initialize();
	// 線
	axis_ = new Axis();
	axis_->Initialize();
}

void TitleScene::Update(GameManager* gameManager) {
	// ImGui
	ImGui::Begin("Settings");
	if (ImGui::BeginTabBar("MyTabBar"))
	{
		if (ImGui::BeginTabItem("Triangle1")) {
			triangle_[0]->ImGuiAdjustParameter();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Triangle2")) {
			triangle_[1]->ImGuiAdjustParameter();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Sprite")) {
			sprite_->ImGuiAdjustParameter();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Sphere")) {
			sphere_->ImGuiAdjustParameter();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Plane")) {
			plane_->ImGuiAdjustParameter();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Axis")) {
			axis_->ImGuiAdjustParameter();
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
	ImGui::End();
}

void TitleScene::Draw(GameManager* gameManager) {
	for (int i = 0; i < kMaxTriangle; i++) {
		triangle_[i]->Draw();
	}
	sphere_->Draw();
	sprite_->Draw();
	plane_->Draw();
	axis_->Draw();
}

void TitleScene::Finalize(GameManager* gameManager) {
	for (int i = 0; i < kMaxTriangle; i++) {
		delete triangle_[i];
	}
	delete sphere_;
	delete sprite_;
	delete plane_;
	delete axis_;
}