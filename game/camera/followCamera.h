#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Input.h"
class FollowCamera {
public:
	void Initialize();

	void Update();
	void SetTarget(const WorldTransform* target);
	const ViewProjection& GetViewProjection() { return viewprojection_; }
	Vector3 GettargetWordPos();
	void Reset();
private:
	ViewProjection viewprojection_;
	const WorldTransform* target_ = nullptr;
	Input* input_ = nullptr;
	Vector3 interTarget_ = {};

	float destinationAngleY_ = 0.0f;
private:
	void Move();
	void Rotate();
};
