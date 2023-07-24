#include "GameScene.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	GameScene* gameScene = new GameScene();

	gameScene->Run();

	delete gameScene;

	return 0;
}