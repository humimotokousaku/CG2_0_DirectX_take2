#include "Camera.h"
#include "WinApp.h"

void Camera::Initialize() {
	cameraTransform_ = {
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,-10.0f}
	};
}

void Camera::SettingCamera() {
	worldMatrix_ = MakeAffineMatrix(cameraTransform_.scale, cameraTransform_.rotate, Vector3{ 0.0f,0.0f,0.0f });
	cameraMatrix_ = MakeAffineMatrix(cameraTransform_.scale, cameraTransform_.rotate, cameraTransform_.translate);
	viewMatrix_ = Inverse(cameraMatrix_);
	projectionMatrix_ = MakePerspectiveFovMatrix(0.45f, (float)WinApp::kClientWidth_ / (float)WinApp::kClientHeight_, 0.1f, 100.0f);
	worldViewProjectionMatrix_ = Multiply(worldMatrix_, Multiply(viewMatrix_, projectionMatrix_));
	transformationMatrixData_ = &worldViewProjectionMatrix_;
}