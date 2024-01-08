#include "PlayerManager.h"

void PlayerManager::Initialize(const std::vector<Model*>& models)
{
	for (int i = 0; i < 4; i++) {
		player_[i] = std::make_unique<Player>();
	}
	player_[0]->Initialize(models, { 50.0f,1.0f,25.0f },Fowerd);
	player_[1]->Initialize(models, { 15.0f,1.0f,8.0f },RightBack);
	player_[2]->Initialize(models, { 30.0f,1.0f,26.0f },MidFirld);
	//player_[3]->Initialize(models, { 15.0f,1.0f,45.0f },LeftBack);
	player_[3]->Initialize(models, { 0.0f,1.0f,25.0f },GoalKeeper);
	selectNum_ = -1;
}

void PlayerManager::Update()
{
	for (int i = 0; i < 4; i++) {
		player_[i]->Update();
	}
	if (Input::GetInstance()->PushButtun(0, XINPUT_GAMEPAD_LEFT_SHOULDER)) {
		if (selectNum_ < 3) {
			selectNum_++;
		}
		else {
			selectNum_ = 0;
		}
	}
	for (int i = 0; i < 4; i++) {
		if (player_[i]->isBall_) {
			selectNum_ = i;
		}
		if (i == selectNum_) {
			player_[i]->SetIsSelected(true);
		}
		else {
			player_[i]->SetIsSelected(false);
		}
	}
}

void PlayerManager::Draw(const ViewProjection& view)
{
	for (int i = 0; i < 4; i++) {
		player_[i]->Draw(view);
	}
}
