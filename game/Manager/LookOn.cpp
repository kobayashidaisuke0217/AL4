#include "LookOn.h"
#include "Enemy.h"
void LookOn::Initialize()
{
	
	
	Mark_ = std::make_unique<Sprite>();
	Mark_->Initialize({0.0f,0.0f,0.0f,0.0f},{100.0f,100.0f,0.0f,0.0f});
	SpriteTransform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f} ,{640.0f,360.0f,1.0f} };
	texhandle_= Texturemanager::GetInstance()->Load("Resource/reticle.png");
	isLockOn_ = false;
	count_ = 0;
}

void LookOn::Update(const std::list<Enemy*>& enemys, const ViewProjection& viewProjection)
{
	XINPUT_STATE joystate;
	count_++;
	if (!Input::GetInstance()->GetJoystickState(0, joystate)) {
		return;
	}
	if (joystate.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
		if (!(preInputPad.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)) {
			if (!target_) {
				isLockOn_ = true;
				Search(enemys, viewProjection);
			}
			else {
				isLockOn_ = false;
			}
			
		}
	}
	
	if (isLockOn_) {
		//Search(enemys, viewProjection);
		
	}
	if (!isLockOn_) {
		Reset();
			
	}
	
	if (target_) {
		if (!target_->GetisAlive()) {
			Reset();
			return;
		}
		Vector3 positionWorld=target_->GetWorldTransform().GetCenter();
	
		Vector3 pos = WorldToScreen(positionWorld, viewProjection);
		SpriteTransform_.translate.x = pos.x;
		SpriteTransform_.translate.y = pos.y;
		if (isRangeOut(viewProjection)) {
			Reset();
		}
	}
	preInputPad = joystate;
}

void LookOn::Draw()
{
	if (target_) {

		Mark_->Draw(SpriteTransform_, { {1.0f,1.0f,1.0f},{1.0f,1.0f,1.0f} ,{1.0f,1.0f,1.0f} }, { 1.0f,1.0f,1.0f,1.0f }, texhandle_);
	}
}



void LookOn::Search(const std::list<Enemy*>& enemys, const ViewProjection& viewProjection)
{
	//if (count_ > 60) {
		std::list<std::pair<float, Enemy*>>targets;
		targets.clear();
		for (Enemy* enemy : enemys) {
			Vector3 positionWorld = enemy->GetWorldTransform().GetCenter();
			Vector3 positionView = vectorTransform(positionWorld, viewProjection.matView);
			if (minDistance_ <= positionView.z && positionView.z <= maxDistance_) {
				float arctangent = std::atan2(std::sqrt(positionView.x * positionView.x + positionView.y * positionView.y), positionView.z);

				if (std::abs(arctangent) <= angleRange_) {
					targets.emplace_back(std::make_pair(positionView.z, enemy));
				}
			}

		}
		target_ = nullptr;
		if (!targets.empty()) {
			targets.sort([](auto& pair1, auto& pair2) {return pair1.first < pair2.first; });
			target_ = targets.front().second;
		}
		count_ = 0;
	//}
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

bool LookOn::isRangeOut(const ViewProjection& viewProjection)
{
	
	Vector3 positionWorld = target_->GetWorldTransform().GetCenter();
	Vector3 positionView = vectorTransform(positionWorld, viewProjection.matView);
	if (minDistance_ <= positionView.z && positionView.z <= maxDistance_) {
		float arctangent = std::atan2(std::sqrt(positionView.x * positionView.x + positionView.y * positionView.y), positionView.z);

		if (std::abs(arctangent) <= angleRange_) {
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