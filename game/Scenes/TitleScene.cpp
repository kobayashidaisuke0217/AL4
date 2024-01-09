#include "TitleScene.h"
#include "ImGuiManger.h"

TitleScene::~TitleScene()
{
}
void TitleScene::Initialize()
{	
	bluemoon_ = BlueMoon::GetInstance();
	input = Input::GetInstance();
	count = 0;
	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize(Texturemanager::GetInstance()->Load("resource/title.png"));

}

void TitleScene::Update()
{
	sprite_->position = { 640.0f,360.0f };
	sprite_->size_ = { 35.0f,26.0f };
	ImGui::Begin("SceneManager");
	ImGui::InputInt("SceneNum", &sceneNum);
	ImGui::Text("count %d",count);
	ImGui::End();
	if (input->PushKey(DIK_SPACE)) {
		sceneNum = GAME_SCENE;
		count++;
	}
	XINPUT_STATE joyState;
	if (!input->GetJoystickState(0, joyState)) {
		return;
	}
	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
		sceneNum = GAME_SCENE;
	}
	if (sceneNum < 0) {
		sceneNum = 0;
		
	}
	/*if (count >= 60) {
		sceneNum=GAME_SCENE;
	}*/
}

void TitleScene::Draw()
{
	ImGui::Begin("TITLE");
	ImGui::Text("PushA:Start");
	ImGui::End();
	bluemoon_->SpritePreDraw();
	sprite_->Draw({
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
		}, { 1.0f,1.0f,1.0f,1.0f });
}

void TitleScene::Finalize()
{
}
