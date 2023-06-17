#pragma once
#include "Vector3.h"
#include "Matrix4x4.h"
#include "Transform.h"

class Camera
{
public:
	
	~Camera();

	// カメラ位置の初期化
	void Initialize();

	// カメラの設定
	void SettingCamera(Matrix4x4 worldMatrix);

private:
	Matrix4x4 cameraMatrix_;
	Transform cameraTransform_;
	Matrix4x4 viewMatrix_;
	Matrix4x4 projectionMatrix_;
	Matrix4x4 worldViewProjectionMatrix_;
	Matrix4x4* transformationMatrixData_;
};
