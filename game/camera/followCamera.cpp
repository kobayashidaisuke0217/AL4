#include "FollowCamera.h"
#include"MyMath.h"
#include "ImguiManger.h"
void FollowCamera::Initialize() {
	viewprojection_.Initialize();
	input_ = Input::GetInstance();
	viewprojection_.translation_ = { 0.0f,7.3f,-2.1f };
	viewprojection_.rotation_ = { 1.2f,0.0f,0.0f };
}

void FollowCamera::Update() {

	Move();
	Rotate();
	viewprojection_.UpdateViewMatrix();
	viewprojection_.TransferMatrix();
}

void FollowCamera::SetTarget(const WorldTransform* target) {

	target_ = target;
}

void FollowCamera::Move() {
	/*if (target_) {

		Vector3 offset = { 0.0f, 2.0f, -10.0f };

		Matrix4x4 rotateMatrix = MakeRotateMatrix(viewprojection_.rotation_);

		offset = TransformNormal(offset, rotateMatrix);
		viewprojection_.translation_ = Add(target_->translation_, offset);
	}*/
	
	ImGui::Begin("camera");
	ImGui::DragFloat3("trans", &viewprojection_.translation_.x, 0.1f);
	ImGui::DragFloat3("rotate", &viewprojection_.rotation_.x, 0.1f);
	ImGui::End();
}

void FollowCamera::Rotate() {
	XINPUT_STATE joystate;

	if (Input::GetInstance()->GetJoystickState(0, joystate)) {
		const float kRotateSpeed = 0.02f;
		viewprojection_.rotation_.y += (float)joystate.Gamepad.sThumbRX / SHRT_MAX * kRotateSpeed;
	}
}
