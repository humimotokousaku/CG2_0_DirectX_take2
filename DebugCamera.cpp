#include "DebugCamera.h"
#include "Input.h"

DebugCamera* DebugCamera::GetInstance() {
	static DebugCamera instance;

	return &instance;
}

void DebugCamera::initialize() {
	// x,y,z軸周りのローカル回転角
	rotation_ = { 0,0,0 };
	// ローカル座標
	translation_ = { 0,0,-50 };
}

void DebugCamera::Update() {
#pragma region 平行移動

	if (Input::GetInstance()->PressKey(DIK_SPACE)) {
		const float speed = 0.1f;

		Vector3 move = { 0,0,speed };

		// 移動ベクトルをカメラの角度だけ回転
		move = TransformNormal(move, worldTransform_);

		translation_ = Add(translation_, move);
	}
	if (Input::GetInstance()->PressKey(DIK_LEFT)) {
		const float speed = -0.1f;

		Vector3 move = { speed,0,0 };

		// 移動ベクトルをカメラの角度だけ回転
		move = TransformNormal(move, worldTransform_);

		translation_ = Add(translation_, move);
	}
	if (Input::GetInstance()->PressKey(DIK_RIGHT)) {
		const float speed = 0.1f;

		Vector3 move = { speed,0,0 };

		// 移動ベクトルをカメラの角度だけ回転
		move = TransformNormal(move, worldTransform_);

		translation_ = Add(translation_, move);
	}
	if (Input::GetInstance()->PressKey(DIK_UP)) {
		const float speed = 0.1f;

		Vector3 move = { 0,speed,0 };

		// 移動ベクトルをカメラの角度だけ回転
		move = TransformNormal(move, worldTransform_);

		translation_ = Add(translation_, move);
	}
	if (Input::GetInstance()->PressKey(DIK_DOWN)) {
		const float speed = -0.1f;

		Vector3 move = { 0,speed,0 };

		// 移動ベクトルをカメラの角度だけ回転
		move = TransformNormal(move, worldTransform_);

		translation_ = Add(translation_, move);
	}

#pragma endregion

#pragma region 回転

	if (Input::GetInstance()->PressKey(DIK_W)) {
		rotation_ = Add(rotation_, { -0.01f,0,0 });
	}
	if (Input::GetInstance()->PressKey(DIK_A)) {
		rotation_ = Add(rotation_, { 0,-0.01f,0 });
	}
	if (Input::GetInstance()->PressKey(DIK_S)) {
		rotation_ = Add(rotation_, { 0.01f,0,0 });
	}
	if (Input::GetInstance()->PressKey(DIK_D)) {
		rotation_ = Add(rotation_, { 0,0.01f,0 });
	}

#pragma endregion

	worldTransform_ = MakeAffineMatrix({ 1,1,1 }, rotation_, translation_);
	viewMatrix_ = Inverse(worldTransform_);
}