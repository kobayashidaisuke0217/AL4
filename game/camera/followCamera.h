#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Input.h"
#include "GlobalVariables.h"
class LookOn;
class FollowCamera {
public:
	void Initialize();

	void Update();
	void SetTarget(const WorldTransform* target);
	const ViewProjection& GetViewProjection() { return viewprojection_; }
	Vector3 GettargetWordPos();
	void Reset();
	void SetlockOn(const LookOn* lockon) { lockOn_ = lockon; }
	void DeletelockOn() { lockOn_ = nullptr; }
private:
	ViewProjection viewprojection_;
	const WorldTransform* target_ = nullptr;
	Input* input_ = nullptr;
	Vector3 interTarget_ = {};
	const LookOn* lockOn_ = nullptr;
	float destinationAngleY_ = 0.0f;
	float delay_;
	Vector3 rotate;
	bool isChange;
	Vector3 changeRotate;
private:
	void Move();
	void Rotate();
	void ApplyGlobalVariables();
	void Change();
};
