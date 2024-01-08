#include "Ball.h"

void Ball::Init()
{
	sphere_ = new Sphere();
	sphere_->Initialize();
	transform_.Initialize();
	transform_.parent_ = nullptr;
	transform_.translation_ = { 30.0f,1.0f,15.0f };
	velocity_ = { 0.0f,0.0f,0.0f };
}

void Ball::Update()
{
	transform_.translation_ = velocity_ + transform_.translation_;
	structSphere_.center = transform_.GetWorldPos();
	structSphere_.radius = 1.0f;
	if (transform_.translation_.z <= 0.0f||transform_.translation_.z>=40.0f) {
		velocity_.z *= -1.0f;
	}
	if (transform_.translation_.x <= 0.0f || transform_.translation_.x >= 75.0f) {
		velocity_.x *= -1.0f;
	}
	if (velocity_.x != 0.0f || velocity_.y != 0.0f || velocity_.z != 0.0f) {
		DeleteParent();
	}
	transform_.UpdateMatrix();
}

void Ball::Draw( ViewProjection& view)
{
	sphere_->Draw({ 1.0f,1.0f,1.0f,1.0f }, transform_, 4, view);
}

Ball* Ball::GetInstance()
{
	static Ball ball;
	return &ball;
}

void Ball::Setparent(const WorldTransform* parent)
{
	if (!transform_.parent_) {
		Vector3 worldPos = transform_.GetWorldPos();
		Vector3 parWorld = { parent->matWorld_.m[3][0],parent->matWorld_.m[3][1],parent->matWorld_.m[3][2] };
		Vector3 Position = worldPos - parWorld;
		transform_.translation_ = Position;
		transform_.parent_ = parent;
	}
}

void Ball::DeleteParent()
{
	if (transform_.parent_) {
		transform_.translation_ = transform_.GetWorldPos();
		transform_.parent_ = nullptr;
	}
}
