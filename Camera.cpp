#include "Camera.h"
#include "WinApp.h"
#include "ImGuiManager.h"

void Camera::Initialize() {
	worldTransform_ = {
	{1.0f,1.0f,1.0f},
	{0.0f,0.0f,0.0f},
	{0.0f,0.0f,0.0f}
	};
	cameraTransform_ = {
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,-10.0f}
	};
}

void Camera::SettingCamera() {
	worldMatrix_ = MakeAffineMatrix(worldTransform_.scale, worldTransform_.rotate, worldTransform_.translate);
	cameraMatrix_ = MakeAffineMatrix(cameraTransform_.scale, cameraTransform_.rotate, cameraTransform_.translate);
	viewMatrix_ = Inverse(cameraMatrix_);
	projectionMatrix_ = MakePerspectiveFovMatrix(0.45f, (float)WinApp::kClientWidth_ / (float)WinApp::kClientHeight_, 0.1f, 100.0f);
	worldViewProjectionMatrix_ = Multiply(worldMatrix_, Multiply(viewMatrix_, projectionMatrix_));
	transformationMatrixData_ = &worldViewProjectionMatrix_;
}

void Camera::DrawDebugParameter() {
	ImGui::Text("Camera");
	ImGui::SliderFloat3("Camera.Translate", &worldTransform_.translate.x, -2, 2);
	ImGui::SliderFloat3("Camera.Rotate", &worldTransform_.rotate.x, -1, 1);
}