#include "Camera.h"

Camera::~Camera(){

}

void Camera::Initialize() {
	cameraTransform_ = {
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,-0.5f}
	};
}

void Camera::SettingCamera(Matrix4x4 worldMatrix) {
	cameraMatrix_ = MakeAffineMatrix(cameraTransform_.scale,cameraTransform_.rotate,cameraTransform_.translate);
	viewMatrix_ = Inverse(cameraMatrix_);
	projectionMatrix_ = MakePerspectiveFovMatrix(0.45f, float(1280) / float(720), 0.1f, 100.0f);
	worldViewProjectionMatrix_ = Multiply(worldMatrix, Multiply(viewMatrix_, projectionMatrix_));
	*transformationMatrixData_ = worldViewProjectionMatrix_;
}