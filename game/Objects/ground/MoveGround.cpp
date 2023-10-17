#include "MoveGround.h"

void MoveGround::Initialize(Model* model, Vector3 translation, Vector3 Scale)
{
	texturemanager_ = Texturemanager::GetInstance();
	worldTransform_.Initialize();
	worldTransform_.translation_ = translation;
	worldTransform_.scale_ = Scale;
	groundModel_ = model;
	groundModel_->SetColor({ 0.0f,1.0f,0.0f,1.0f });
	move_ = { 0.0f,0.0f,0.2f };
}

void MoveGround::Update()
{
	Move();
	if (worldTransform_.translation_.z >= 35.0f || worldTransform_.translation_.z <= 13.0f) {
		move_ = Multiply(-1.0f, move_);
	}
	
	worldTransform_.UpdateMatrix();
}

void MoveGround::Draw(const ViewProjection& view)
{
	groundModel_->Draw(worldTransform_, view);
}


void MoveGround::Move()
{
	worldTransform_.translation_ =Add( worldTransform_.translation_,move_);
}
