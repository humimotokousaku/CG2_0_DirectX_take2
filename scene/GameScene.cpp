#include "GameScene.h"
#include "../Manager/GameManager.h"

void GameScene::Initialize() {
	for (int i = 0; i < kMaxTriangle; i++) {
		triangle_[i] = new Triangle({ -0.3f, -0.2f * (i + 1), 0.0f, 1.0f }, { 0.0f, 0.2f * (i + 1), 0.0f, 1.0f }, { 0.3f, -0.2f * (i + 1), 0.0f, 1.0f });
		triangle_[i]->Initialize();
	}
}

void GameScene::Update() {

	if (ImGui::BeginTabBar("GameSceneTabBar"))
	{
		if (ImGui::BeginTabItem("Triangle1")) {
			triangle_[0]->ImGuiAdjustParameter();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Triangle2")) {
			triangle_[1]->ImGuiAdjustParameter();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Triangle3")) {
			triangle_[2]->ImGuiAdjustParameter();
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}

	ImGui::Begin("CommonSettings");
	if (ImGui::BeginTabBar("CommonTabBar"))
	{
		// シーンの切り替え
		if (ImGui::BeginTabItem("GameScene")) {
			ImGui::Text("Scene : GamePlay");
			ImGui::InputInt("SceneNumber", &sceneNum);
			ImGui::EndTabItem();
		}
		ImGui::EndTabItem();
	}
	ImGui::End();
	if (sceneNum >= 1) {
		sceneNum = 1;
	}
}

void GameScene::Draw() {
	for (int i = 0; i < kMaxTriangle; i++) {
		triangle_[i]->Draw();
	}
}

void GameScene::Finalize() {
	for (int i = 0; i < kMaxTriangle; i++) {
		delete triangle_[i];
	}
}