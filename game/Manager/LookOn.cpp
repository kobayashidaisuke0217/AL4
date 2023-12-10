
#include "LookOn.h"
#include "Enemy.h"
#include  "ImGuiManger.h"
void LookOn::Initialize()
{
	
	
	Mark_ = std::make_unique<Sprite>();
	Mark_->Initialize({0.0f,0.0f,0.0f,0.0f},{100.0f,100.0f,0.0f,0.0f});
	SpriteTransform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f} ,{640.0f,360.0f,1.0f} };
	texhandle_= Texturemanager::GetInstance()->Load("Resource/reticle.png");
	isLockOn_ = false;
	count_ = 0;
	cooltime = 0;
	targetIndex = 0;
}

void LookOn::Update(const std::list<Enemy*>& enemys, const ViewProjection& viewProjection)
{
	

	count_++;

	if (!Input::GetInstance()->GetJoystickState(0, joystate)) {
		return;
	}
	if (joystate.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) {
		if (!(preInputPad.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)) {
			if (isAut) {
				isAut = false;
			}
			else {
				isAut = true;
			}
		}
	}
	if (isAut) {
		AutLock(enemys, viewProjection);
	}
	else {
		SelectLock(enemys, viewProjection);
	}
	if (target_) {
		Vector3 positionWorld = target_->GetWorldTransform().GetCenter();

		Vector3 pos = WorldToScreen(positionWorld, viewProjection);
		SpriteTransform_.translate.x = pos.x;
		SpriteTransform_.translate.y = pos.y;
	}
	preInputPad = joystate;
	count_++;
}

void LookOn::Draw()
{
	if (target_) {

		Mark_->Draw(SpriteTransform_, { {1.0f,1.0f,1.0f},{1.0f,1.0f,1.0f} ,{1.0f,1.0f,1.0f} }, { 1.0f,1.0f,1.0f,1.0f }, texhandle_);
	}
}



void LookOn::Search(const std::list<Enemy*>& enemys, const ViewProjection& viewProjection)
{
	
	
	if (count_ > 60) {


		target_ = nullptr;
		targets.clear();

		for ( Enemy* enemy : enemys) {
			Vector3 positionWorld = enemy->GetWorldTransform().GetCenter();
			Vector3 positionView = vectorTransform(positionWorld, viewProjection.matView);

			if (minDistance_ <= positionView.z && positionView.z <= maxDistance_) {
				Vector3 viewXZ = Normalise({ positionView.x,0.0f,positionView.z });
				Vector3 viewZ = Normalise({ 0.0f,0.0f,positionView.z });
				float cos = Length(Cross(viewXZ, viewZ));

				if (std::abs(cos) <= angleRange_) {
					targets.emplace_back(std::make_pair(positionView.z, enemy));
				}
			}

		}
		target_ = nullptr;
		if (targets.size() != 0) {
			std::sort(targets.begin(), targets.end(), [](auto& pair1, auto& pair2) {return pair1.first < pair2.first; });
			target_ = targets.front().second;
		}

		count_ = 0;
	}
		
		
		
	
}

void LookOn::Reset()
{
	if (target_) {
		target_ = nullptr;
		isLockOn_ = false;
		
	}
	
}

Vector3 LookOn::GetTargetPos() const
{
	{
		if (target_) {
			return target_->GetWorldTransform().GetCenter();
		}
		return Vector3();
	}
}

bool LookOn::isTarget()
{
	if (target_) {
		return true;
	}
	return false;
}

void LookOn::AutLock(const std::list<Enemy*>& enemys, const ViewProjection& viewProjection)
{
	if (joystate.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
		if (!(preInputPad.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)) {
			if (isLockOn_) {
				isLockOn_ = false;
				Reset();
			}
			else {
				isLockOn_ = true;
			}
		}
	}
	if (target_ && isRangeOut(viewProjection)) {
		Reset();
	}
	if (target_ && !target_->GetisAlive()) {
		Reset();
	}
	if (isLockOn_) {
		Search(enemys, viewProjection);
	}
}

void LookOn::SelectLock(const std::list<Enemy*>& enemys, const ViewProjection& viewProjection)
{
	if (target_) {
		if (joystate.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
			if (!(preInputPad.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)) {
				if (isLockOn_) {
					isLockOn_ = false;
					Reset();
				}
				else {
					isLockOn_ = true;
				}
			}
		}
		if (target_ && isRangeOut(viewProjection)) {
			Reset();
		}
		if (target_ && !target_->GetisAlive()) {
			Reset();
		}
		if (joystate.Gamepad.wButtons & XINPUT_GAMEPAD_Y) {
			if (!(preInputPad.Gamepad.wButtons & XINPUT_GAMEPAD_Y)) {
				Search(enemys, viewProjection);
				targetIndex++;
				if (targetIndex >= targets.size()) {
					targetIndex = 0;
					target_ = nullptr;
					target_ = targets[targetIndex].second;
				}
				else {
					target_ = nullptr;
					target_ = targets[targetIndex].second;
				}
			}
		}
	}
}

bool LookOn::isRangeOut(const ViewProjection& viewProjection)
{
	
	Vector3 positionWorld = target_->GetWorldTransform().GetCenter();
	Vector3 positionView = vectorTransform(positionWorld, viewProjection.matView);
	if (minDistance_ <= positionView.z && positionView.z <= maxDistance_) {
		Vector3 viewXZ = Normalise({ positionView.x,0.0f,positionView.z });
		Vector3 viewZ = Normalise({ 0.0f,0.0f,positionView.z });
		float dot = Dot({ 0.0f,0.0f,1.0f }, positionView);
		float length = Length(positionView);
		float cos = Length(Cross(viewXZ, viewZ));

		//cos = Angle({ 0.0f,0.0f,1.0f }, viewXZ);
	//	if (cos <= angleRange_) {

		if (std::abs(cos) <= angleRange_) {
			//範囲内
			return false;
		}
	}
	//範囲外
	return true;
}

Vector3 LookOn::WorldToScreen(Vector3 world, const ViewProjection& viewProjection)
{
	Matrix4x4 matViewport =
		MakeViewportMatrix(0.0f, 0.0f, WinApp::kClientWidth, WinApp::kClientHeight, 0.0f, 1.0f);
	Matrix4x4 matViewProjectionViewport =
		Multiply(viewProjection.matView,Multiply (viewProjection.matProjection, matViewport));
	return vectorTransform(world, matViewProjectionViewport);
}