#include "GameScene.h"
#include "../Manager/GameManager.h"

void GameScene::Initialize(GameManager* gameManager) {
	for (int i = 0; i < kMaxTriangle; i++) {
		triangle_[i] = new Triangle({ -0.3f, -0.2f * (i + 1), 0.0f, 1.0f }, { -0.2f, -0.1f * (i + 1), 0.0f, 1.0f }, { -0.1f, -0.2f * (i + 1), 0.0f, 1.0f });
		triangle_[i]->Initialize();
	}
}

void GameScene::Update(GameManager* gameManager) {

}

void GameScene::Draw(GameManager* gameManager) {
	for (int i = 0; i < kMaxTriangle; i++) {
		triangle_[i]->Draw();
	}
}

void GameScene::Finalize(GameManager* gameManager) {
	for (int i = 0; i < kMaxTriangle; i++) {
		delete triangle_[i];
	}
}