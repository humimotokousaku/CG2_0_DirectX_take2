#pragma once
#include "DirectXCommon.h"
#include "WinApp.h"
#include "Light.h"
#include "ImGuiManager.h"
#include "ObjModel.h"
#include "TextureManager.h"
#include "Camera.h"

class Adapter
{
public:
	void Init

public:
	static WinApp* winApp_;
	static DirectXCommon* directXCommon_;
	static ImGuiManager* imGuiManager_;
	static Camera* camera_;
};

