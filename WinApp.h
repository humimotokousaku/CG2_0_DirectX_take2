#pragma once
#include <Windows.h>
#include <stdint.h>
#include <string>
#include <d3d12.h>

class WinApp
{
public:
	// ウィンドウサイズ
	static const int kWindowWidth = 1280;
	static const int kWindowHeight = 720;
public: // メンバ関数
	// ウィンドウプロシージャ
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	// ウィンドウの生成
	static void CreateGameWindow(const wchar_t* title, int32_t kClientWidth, int32_t kClientHeight);

	// デバッグレイヤ
	static void DebugLayer();

	// Windowsの初期化
	static void Initialize(const wchar_t* title, int32_t kClientWidth = kWindowWidth, int32_t kClientHeight = kWindowHeight);

	// 出力ウィンドウに文字を出す
	static void Log(const std::string& message);

public: // メンバ変数
	// ウィンドウクラス登録用
	static WNDCLASS wc_;

	// ウィンドウを生成
	static HWND hwnd_;
};

