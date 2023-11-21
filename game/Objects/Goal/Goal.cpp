#include "Goal.h"

void Goal::Initialize(Vector3 transform, Vector3 scale)
{
	worldTransform_.Initialize();
	sphere_ = std::make_unique<Sphere>();
	sphere_->Initialize();
	worldTransform_.translation_ = transform;
	worldTransform_.scale_ = scale;
	SetCollisionAttribute(CollisionConfig::kCollisionAttributeEnemy);
	SetCollisionMask(~CollisionConfig::kCollisionAttributeEnemy);
}

void Goal::Update()
{
	worldTransform_.UpdateMatrix();
}

void Goal::Draw(const ViewProjection& view)
{
	sphere_->Draw({ 1.0f,1.0f,1.0f,1.0f }, worldTransform_, 2, view);
}

void Goal::OnCollision()
{
}
