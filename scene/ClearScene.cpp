#include "ClearScene.h"
#include "../GameManager.h"

void ClearScene::Initialize(GameManager* gameManager) {
	for (int i = 0; i < kMaxTriangle; i++) {
		triangle_[i] = new Triangle({ -0.3f, -0.2f * (i + 1), 0.0f, 1.0f }, { -0.2f, -0.1f * (i + 1), 0.0f, 1.0f }, { -0.1f, -0.2f * (i + 1), 0.0f, 1.0f });
		triangle_[i]->Initialize();
	}
}

void ClearScene::Update(GameManager* gameManager) {

}

void ClearScene::Draw(GameManager* gameManager) {
	for (int i = 0; i < kMaxTriangle; i++) {
		triangle_[i]->Draw();
	}
}

void ClearScene::Finalize(GameManager* gameManager) {
	for (int i = 0; i < kMaxTriangle; i++) {
		triangle_[i]->Finalize();
		delete triangle_[i];
	}
}