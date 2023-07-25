#include "TitleScene.h"
#include "../GameManager.h"

void TitleScene::Initialize(GameManager* gameManager) {
	for (int i = 0; i < kMaxTriangle; i++) {
		triangle_[i] = new Triangle({ -0.3f, -0.2f * (i + 1), 0.0f, 1.0f }, { -0.2f, -0.1f * (i + 1), 0.0f, 1.0f }, { -0.1f, -0.2f * (i + 1), 0.0f, 1.0f });
		triangle_[i]->Initialize();
	}
}

void TitleScene::Update(GameManager* gameManager) {

}

void TitleScene::Draw(GameManager* gameManager) {
	for (int i = 0; i < kMaxTriangle; i++) {
		triangle_[i]->Draw();
	}
}

void TitleScene::Finalize(GameManager* gameManager) {
	for (int i = 0; i < kMaxTriangle; i++) {
		triangle_[i]->Finalize();
		delete triangle_[i];
	}
}