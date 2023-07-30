#include "TitleScene.h"
#include "../Manager/GameManager.h"

void TitleScene::Initialize(GameManager* gameManager) {
	for (int i = 0; i < kMaxTriangle; i++) {
		triangle_[i] = new Triangle({ -1.0f, -0.4f * (i + 1), 0.0f, 1.0f }, { 0.0f, -0.1f * (i + 1), 0.0f, 1.0f }, { 1.0f, -0.4f * (i + 1), 0.0f, 1.0f });
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
}

void TitleScene::Update(GameManager* gameManager) {
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
		if (ImGui::BeginTabItem("MultiMesh")) {
			multiMesh_->ImGuiAdjustParameter();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("MultiMaterial")) {
			multiMaterial_->ImGuiAdjustParameter();
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
	multiMesh_->Draw();
	multiMaterial_->Draw();
}

void TitleScene::Finalize(GameManager* gameManager) {
	for (int i = 0; i < kMaxTriangle; i++) {
		delete triangle_[i];
	}
	delete sphere_;
	delete sprite_;
	delete plane_;
	delete axis_;
	delete multiMesh_;
	delete multiMaterial_;
}