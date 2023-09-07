#include "WorldTransform.h"
#include <cassert>

void WorldTransform::Initialize() {
	matWorld_ = MakeIdentity4x4();
	CreateConstBuffer();
	Map();
	TransferMatrix();
}

void WorldTransform::CreateConstBuffer() {
	constBuff_ = DirectXCommon::GetInstance()->CreateBufferResource(sizeof(ConstBufferDataWorldTransform));
}

void WorldTransform::Map() {
	constBuff_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&constMap));
}

void WorldTransform::TransferMatrix() {
	constMap->matWorld = matWorld_;
}

void WorldTransform::UpdateMatrix() {
	Matrix4x4 affineMatrix = MakeAffineMatrix(scale_, rotation_, translation_);
	matWorld_ = affineMatrix;

	// 親子関係の計算
	if (parent_) {
		matWorld_ = Multiply(matWorld_, parent_->matWorld_);
	}

	TransferMatrix();
}