#include "./Manager/GameManager.h"
#include <xaudio2.h>
#pragma comment(lib, "xaudio2.lib")
#include <fstream>

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	std::unique_ptr<GameManager> gameManager = std::make_unique<GameManager>();
	gameManager->Run();
	return 0;
}