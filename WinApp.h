#pragma once
#include <Windows.h>
#include <stdint.h>
#include <string>

// ここではWindows初期化のクラスを扱っている
class WinApp
{
public:
	// メンバ関数
	WinApp();
	~WinApp();

	// ウィンドウプロシージャ
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	// ウィンドウクラスの登録
	static void WindowClassRegister();

	// ウィンドウサイズを決める
	static void WindowSizeDecide();

	// ウィンドウの生成
	static void WindowGeneration();

	// Windowsの初期化
	static void Initialize();

	// 出力ウィンドウに文字を出す
	static void Log(const std::string& message);

public:
	// メンバ変数

	// ウィンドウクラス登録用
	static WNDCLASS wc_;

	// クライアント領域のサイズ
	static const int32_t kClientWidth_ = 1280;
	static const int32_t kClientHeight_ = 720;

	static RECT wrc_;

	// ウィンドウを生成
	static HWND hwnd_;

};

