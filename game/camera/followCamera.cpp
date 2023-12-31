#include "FollowCamera.h"
#include"MyMath.h"
#include "ImguiManger.h"
void FollowCamera::Initialize() {
	viewprojection_.Initialize();
	input_ = Input::GetInstance();
	GlovalVariables* globalVariables{};
	globalVariables = GlovalVariables::GetInstance();
	const char* groupName = "Camera";
	GlovalVariables::GetInstance()->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "delay", delay_);
	ApplyGlobalVariables();
	
}

void FollowCamera::Update() {
	ApplyGlobalVariables();
	Move();
	Rotate();
	viewprojection_.UpdateViewMatrix();
	viewprojection_.TransferMatrix();
}

void FollowCamera::SetTarget(const WorldTransform* target) {

	target_ = target;
}

Vector3 FollowCamera::GettargetWordPos()
{
	Vector3 result;
	result.x = target_->matWorld_.m[3][0];
	result.y = target_->matWorld_.m[3][1];
	result.z = target_->matWorld_.m[3][2];
	return result;
}

void FollowCamera::Move() {
	if (target_) {

		Vector3 offset = { 0.0f, 2.0f, -10.0f };

		Matrix4x4 rotateMatrix = MakeRotateMatrix(viewprojection_.rotation_);

		offset = TransformNormal(offset, rotateMatrix);
		Vector3 worldTranslate= { target_->matWorld_.m[3][0],target_->matWorld_.m[3][1],target_->matWorld_.m[3][2] };
		
		interTarget_ = Lerp(delay_, worldTranslate, interTarget_);
		viewprojection_.translation_ = interTarget_ + offset;
	}
	
}

void FollowCamera::Rotate() {
	XINPUT_STATE joystate;

	if (Input::GetInstance()->GetJoystickState(0, joystate)) {
		const float kRotateSpeed = 0.02f;
		viewprojection_.rotation_.y += (float)joystate.Gamepad.sThumbRX / SHRT_MAX * kRotateSpeed;
	}
}
void FollowCamera::ApplyGlobalVariables()
{
	GlovalVariables* globalVariables = GlovalVariables::GetInstance();

	const char* groupName = "Camera";
	delay_= globalVariables->GetFloatValue(groupName, "delay");
	if (delay_ >= 1.0f) {
		delay_ = 0.9999999f;
	}
	if (delay_ <= 0.0f) {
		delay_ = 0.0f;
	}
}
void FollowCamera::Reset() {
	if (target_) {
		Vector3 worldTranslate = { target_->matWorld_.m[3][0],target_->matWorld_.m[3][1],target_->matWorld_.m[3][2] };
		interTarget_ = worldTranslate;
		viewprojection_.rotation_.y = target_->rotation_.y;
	}
	destinationAngleY_ = viewprojection_.rotation_.y;

}
