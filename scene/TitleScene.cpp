#include "TitleScene.h"
#include "../Manager/GameManager.h"

void TitleScene::Initialize() {
	block_ = new Block();
	block_->Initialize();
	textureNum_ = UVCHEKER;
	input_ = Input::GetInstance();
	worldTransform_.Initialize();
	viewProjection_.Initialize();
}

void TitleScene::Update() {

	worldTransform_.UpdateMatrix();

	// 追従対象からカメラまでのオフセット
	Vector3 offset = { 0.0f, 4.0f, -10.0f };
	// カメラの角度から回転行列を計算
	Matrix4x4 rotateMatrix = MakeRotateMatrix(viewProjection_.rotation_);

	// オフセットをカメラの回転に合わせて回転
	offset = TransformNormal(offset, rotateMatrix);

	// 座標をコピーしてオフセット分ずらす
	viewProjection_.translation_ = Add(worldTransform_.translation_, offset);
	XINPUT_STATE joyState;
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		//// デッドゾーンの設定
		SHORT rightThumbX = Input::GetInstance()->ApplyDeadzone(joyState.Gamepad.sThumbRX);
		const float kRadian = 0.02f;
		viewProjection_.rotation_.y += (float)rightThumbX / SHRT_MAX * kRadian;
	}

	viewProjection_.UpdateViewMatrix();
	viewProjection_.TransferMatrix();

	ImGui::Begin("block");
	ImGui::DragFloat3("translation", &worldTransform_.translation_.x, 0.1f);
	ImGui::DragFloat3("rotation", &worldTransform_.rotation_.x, 0.1f);
	ImGui::DragFloat3("scale", &worldTransform_.scale_.x, 0.1f);
	ImGui::End();
}

void TitleScene::Draw() {
	block_->Draw(worldTransform_, viewProjection_);
}

void TitleScene::Finalize() {
	delete block_;
	worldTransform_.constBuff_.ReleaseAndGetAddressOf();
	viewProjection_.constBuff_.ReleaseAndGetAddressOf();
}