#include "GameScene.h"
#include "../Manager/GameManager.h"

void GameScene::Initialize() {
	for (int i = 0; i < kMaxTriangle; i++) {
		triangle_[i] = new Triangle({ -0.3f, -0.2f * (i + 1), 0.0f, 1.0f }, { -0.2f, -0.1f * (i + 1), 0.0f, 1.0f }, { -0.1f, -0.2f * (i + 1), 0.0f, 1.0f });
		triangle_[i]->Initialize();
	}
}

void GameScene::Update() {
	if (ImGui::BeginTabBar("CommonTabBar"))
	{
		// シーンの切り替え
		if (ImGui::BeginTabItem("GameScene")) {
			ImGui::Text("Scene : Game");
			ImGui::InputInt("SceneNumber", &sceneNum);
			ImGui::EndTabItem();
		}
		ImGui::EndTabItem();
	}
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