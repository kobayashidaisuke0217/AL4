#include "MoveGround.h"

void MoveGround::Initialize(Model* model, Vector3 translation, Vector3 Scale)
{
	texturemanager_ = Texturemanager::GetInstance();
	worldTransform_.Initialize();
	worldTransformBase_.Initialize();
	worldTransformBase_.translation_ = translation;
	worldTransform_.scale_ = Scale;
	groundModel_ = model;
	groundModel_->SetColor({ 0.0f,1.0f,0.0f,1.0f });
	SetParent(&GetWorldTransformBase_());
	move_ = { 0.0f,0.0f,0.2f };
}

void MoveGround::Update()
{
	Move();
	if (worldTransform_.translation_.z >= 10.0f || worldTransform_.translation_.z <= -10.0f) {
		move_ = Multiply(-1.0f, move_);
	}
	worldTransformBase_.UpdateMatrix();
	worldTransform_.UpdateMatrix();
}

void MoveGround::Draw(const ViewProjection& view)
{
	groundModel_->Draw(worldTransform_, view);
}

void MoveGround::SetParent(const WorldTransform* parent)
{
	worldTransform_.parent_ = parent;
}

void MoveGround::Move()
{
	worldTransform_.translation_ =Add( worldTransform_.translation_,move_);
}
