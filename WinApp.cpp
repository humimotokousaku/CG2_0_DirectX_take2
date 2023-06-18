#include "WinApp.h"
#include <Windows.h>
#include <cstdint>
#include <string>
#include <format>

#include "externals/ImGui/imgui.h"
#include "externals/ImGui/imgui_impl_dx12.h"
#include "externals/ImGui/imgui_impl_win32.h"
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

// ウィンドウプロシージャ
LRESULT CALLBACK WinApp::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam)) {
		return true;
	}
	// メッセージに応じてゲーム固有の処理を行う
	switch (msg) {
		// ウィンドウが破棄された
	case WM_DESTROY:
		// OSに対して、アプリの終了を伝える
		PostQuitMessage(0);
		return 0;
	}

	// 標準のメッセージ処理を行う
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

// ウィンドウクラスの登録
void WinApp::WindowClassRegister() {
	// ウィンドウプロシージャ
	wc_.lpfnWndProc = WindowProc;
	// ウィンドウクラス名
	wc_.lpszClassName = L"CG2WindowClass";
	// インスタンスハンドル
	wc_.hInstance = GetModuleHandle(nullptr);
	// カーソル
	wc_.hCursor = LoadCursor(nullptr, IDC_ARROW);

	// ウィンドウクラスを登録する
	RegisterClass(&wc_);
}

// ウィンドウサイズを決める
void WinApp::WindowSizeDecide() {

	// ウィンドウサイズを表す構造体にクライアント領域を入れる
	wrc_ = { 0, 0, kClientWidth_, kClientHeight_ };

	// クライアント領域をもとに2サイズにWRCを変更してもらう
	AdjustWindowRect(&wrc_, WS_OVERLAPPEDWINDOW, false);
}

// ウィンドウの生成
void WinApp::WindowGeneration() {
	hwnd_ = CreateWindow(
		wc_.lpszClassName,
		L"CG2_CLASS",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		wrc_.right - wrc_.left,
		wrc_.bottom - wrc_.top,
		nullptr,
		nullptr,
		wc_.hInstance,
		nullptr
	);
}

void WinApp::DebugLayer() {
#ifdef _DEBUG
	//debugController_ = nullptr;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController_)))) {
		// デバッグレイヤーを有効化する
		debugController_->EnableDebugLayer();
		// さらにGPU側でもチェックを行うようにする
		debugController_->SetEnableGPUBasedValidation(TRUE);
	}
#endif
}

// Windowsの初期化
void WinApp::Initialize() {
	WindowClassRegister();
	WindowSizeDecide();
	WindowGeneration();
	// ウィンドウを表示する
	ShowWindow(hwnd_, SW_SHOW);
}

// 出力ウィンドウに文字を出す
void WinApp::Log(const std::string& message) {
	OutputDebugStringA(message.c_str());
}

#pragma region メンバ変数

ID3D12Debug1* WinApp::debugController_;

// ウィンドウクラス登録用
WNDCLASS WinApp::wc_;

RECT WinApp::wrc_;

// ウィンドウを生成
HWND WinApp::hwnd_;

#pragma endregion