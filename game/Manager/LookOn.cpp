
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
				Target();
			}
			else {
				isLockOn_ = false;
			}
			
		}
	}
	if (joystate.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) {
		if (!(preInputPad.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)) {
			if (isAut) { 
				isAut = false;
			Search(enemys, viewProjection);
		
			}
			else {
				isAut = true;
				Search(enemys, viewProjection);
				
			}
		}
	}
	
	
		
		if (joystate.Gamepad.wButtons & XINPUT_GAMEPAD_Y) {
			if (!(preInputPad.Gamepad.wButtons & XINPUT_GAMEPAD_Y)) {
				isLockOn_ = true;
				//Search(enemys, viewProjection);
				if (iteratornum > 0) {

					iteratornum--;
					Search(enemys, viewProjection);
					Target();
				}
				else {
					iteratornum = max;
					Search(enemys, viewProjection);
					Target();
				}
			}
		}
	
	/*if (isAut) {
		Search(enemys, viewProjection);
		Target();
		
	}*/
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
		if (isAut) {
			if (isRangeOut(viewProjection)) {
				Reset();
				Search(enemys, viewProjection);
				Target();
			}
		}
	}
	preInputPad = joystate;
	ImGui::Begin("LockOn");
	ImGui::Checkbox("IsAut", &isAut);
	ImGui::End();
	count_++;
}

void LookOn::Draw()
{
	if (target_) {

		Mark_->Draw(SpriteTransform_, { {1.0f,1.0f,1.0f},{1.0f,1.0f,1.0f} ,{1.0f,1.0f,1.0f} }, { 1.0f,1.0f,1.0f,1.0f }, texhandle_);
	}
}

void LookOn::Target()
{
	target_ = nullptr;

	if (!targets.empty()) {
		targets.sort([](auto& pair1, auto& pair2) {return pair1.first > pair2.first; });
		max = (int)targets.size();
		if (isAut == true) { target_ = targets.front().second; }
		else {
			auto it = targets.begin(); // イテレータをリストの先頭に設定する
			if (iteratornum >= targets.size()) {
				iteratornum = (int)targets.size();
			}

			std::advance(it, iteratornum);
			if (it != targets.end()) {
				std::pair<float, Enemy*>element = *it;
				target_ = element.second;
			}


		}
	}
}



void LookOn::Search(const std::list<Enemy*>& enemys, const ViewProjection& viewProjection)
{
	if (count_ > 60) {
	
		
		target_ = nullptr;
			targets.clear();
			for (Enemy* enemy : enemys) {
				Vector3 positionWorld = enemy->GetWorldTransform().GetCenter();
				Vector3 positionView = vectorTransform(positionWorld, viewProjection.matView);
				if (minDistance_ <= positionView.z && positionView.z <= maxDistance_) {
					//	float arctangent = std::atan2(std::sqrt(positionView.x * positionView.x + positionView.y * positionView.y), positionView.z);
					Vector3 viewXZ = Normalise({ positionView.x,0.0f,positionView.z });
					Vector3 viewZ = Normalise({ 0.0f,0.0f,positionView.z });
					float cos = Length(Cross(viewXZ, viewZ));
					
					if (std::abs(cos) <= angleRange_) {
						
						targets.emplace_back(std::make_pair(positionView.z, enemy));
					}
				}

			}
		
		
		
		count_ = 0;
	}
}

void LookOn::Reset()
{
	if (target_) {
		target_ = nullptr;
		isLockOn_ = false;
		iteratornum = 0;
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
		Vector3 viewXZ = Normalise({ positionView.x,0.0f,positionView.z });
		Vector3 viewZ = Normalise({ 0.0f,0.0f,positionView.z });

		float cos = Length(Cross(viewXZ, viewZ));
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