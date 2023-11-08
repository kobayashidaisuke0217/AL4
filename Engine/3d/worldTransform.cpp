#include "worldTransform.h"

void WorldTransform::Initialize()
{
	matWorld_ = MakeIdentity4x4();
	CreateConstBuffer();
	Map();
	TransferMatrix();
	/*quaternion_ = createQuaternion(0.0f, { 0.0f,1.0f,0.0f });
	quaternion_ = Normalize(quaternion_);*/
}

void WorldTransform::CreateConstBuffer()
{
	constBuff_ = DirectXCommon::GetInstance()->CreateBufferResource(DirectXCommon::GetInstance()->GetDevice().Get(), sizeof(ConstBufferDataWorldTransform));

}

void WorldTransform::Map()
{
	constBuff_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&constMap));

}

void WorldTransform::TransferMatrix()
{
	constMap->matWorld = matWorld_;
}

void WorldTransform::UpdateMatrix()
{
	Matrix4x4 AffineMatrix = MakeAffineMatrix(scale_, rotation_, translation_);
	matWorld_ = AffineMatrix;
	//親があれば親のワールド行列を掛ける
	if (parent_) {
		matWorld_ = Multiply(matWorld_, parent_->matWorld_);
	}

	TransferMatrix();
}
void WorldTransform::UpdateRotateMatrix(const Matrix4x4& rotateMat)
{
	Matrix4x4 AffineMatrix = MakeRotateAffineMatrix(scale_, rotateMat, translation_);
	matWorld_ = AffineMatrix;
	//親があれば親のワールド行列を掛ける
	if (parent_) {
		matWorld_ = Multiply(matWorld_, parent_->matWorld_);
	}

	TransferMatrix();
}
Vector3 WorldTransform::GetWorldPos()
{
	return { matWorld_.m[3][0],matWorld_.m[3][1],matWorld_.m[3][2] };
}
void WorldTransform::UpdateQuaternionMatrix()
{
	Matrix4x4 quart_ = quaternionToMatrix(quaternion_);
	Matrix4x4 Afine = MakeQuatAffineMatrix(scale_, quart_, translation_);
	matWorld_ = Afine;
	if (parent_) {
		matWorld_ = Multiply(matWorld_, parent_->matWorld_);
	}
	TransferMatrix();
}