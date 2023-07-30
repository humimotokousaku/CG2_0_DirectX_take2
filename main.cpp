#include "./Manager/GameManager.h"

const char kWindowTitle[] = "CG2_CLASS";

struct D3DResourceLeakCheker {
	~D3DResourceLeakCheker() {
		// リリースリークチェック
		Microsoft::WRL::ComPtr<IDXGIDebug1> debug;
		if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debug)))) {
			debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
			debug->ReportLiveObjects(DXGI_DEBUG_APP, DXGI_DEBUG_RLO_ALL);
			debug->ReportLiveObjects(DXGI_DEBUG_D3D12, DXGI_DEBUG_RLO_ALL);
		}
	}
};

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	//D3DResourceLeakCheker leakCheck;
	GameManager* gameManager = new GameManager();
	gameManager->Run();
	delete gameManager;
	return 0;
}