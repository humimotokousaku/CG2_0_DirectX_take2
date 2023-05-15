#include "MyWindows.h"
#include <Windows.h>
#include <cstdint>
#include <string>
#include <format>

MyWindows::MyWindows() {

}

MyWindows::~MyWindows() {

}

// ウィンドウプロシージャ
LRESULT CALLBACK MyWindows::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
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
void MyWindows::WindowClassRegister() {
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
void MyWindows::WindowSizeDecide() {

	// ウィンドウサイズを表す構造体にクライアント領域を入れる
	wrc_ = { 0, 0, kClientWidth_, kClientHeight_ };

	// クライアント領域をもとに2サイズにWRCを変更してもらう
	AdjustWindowRect(&wrc_, WS_OVERLAPPEDWINDOW, false);
}

// ウィンドウの生成
void MyWindows::WindowGeneration() {
	hwnd_ = CreateWindow(
		wc_.lpszClassName,
		L"CG2_revenge",
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

// 出力ウィンドウに文字を出す
void MyWindows::Log(const std::string& message) {
	OutputDebugStringA(message.c_str());
}

#pragma region メンバ変数

// ウィンドウクラス登録用
WNDCLASS MyWindows::wc_;

RECT MyWindows::wrc_;

// ウィンドウを生成
HWND MyWindows::hwnd_;

#pragma endregion