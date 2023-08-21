#pragma once

#define DIRECTINPUT_VERSION		0x0800  // DirectInputのバーしょん指定
#include <dinput.h>
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#include "base/WinApp.h"
#include <array>
#include <windows.h>
#include <Xinput.h>

class Input
{
public:
	static Input* GetInstance();

	void Initialize();

	void Update();

	// キーを押した瞬間
	bool TriggerKey(BYTE keyNumber) const;

	// キーを押している間
	bool PressKey(BYTE keyNumber)const;

	// キーを離した瞬間
	bool ReleaseKey(BYTE keyNumber)const;

	bool GetJoystickState(int32_t stickNo, XINPUT_STATE& state);

private:
	IDirectInput8* directInput_ = nullptr;
	IDirectInputDevice8* keyboard_ = nullptr;
	std::array<BYTE, 256> key_;
	std::array<BYTE, 256> preKey_;
};

