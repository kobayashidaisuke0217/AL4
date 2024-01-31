#include "TitleScene.h"
#include "ImGuiManger.h"

TitleScene::~TitleScene()
{
}
void TitleScene::Initialize()
{	
	bluemoon_ = BlueMoon::GetInstance();
	input = Input::GetInstance();
	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize(Texturemanager::GetInstance()->Load("resource/title.png"));
	fade_ = Fade::GetInstance();
	fade_->Initialize();
	sprite_->size_ = { 0.0f,0.0f };
	sprite_->position = { 640.0f,360.0f };
	count = 0.0f;
}

void TitleScene::Update()
{
	if (count < 1.0f) {
		count += 0.05f;
	}
	if (count > 1.0f) {
		count = 1.0f;
	}
	sprite_->size_.x = Lerp(count,0.0, 35.0f);
	sprite_->size_.y = Lerp(count, 0.0, 26.0f);
	ImGui::Begin("SceneManager");
	ImGui::InputInt("SceneNum", &sceneNum);
	ImGui::Text("count %d",count);
	ImGui::End();
	if (input->PushKey(DIK_SPACE)) {
		fade_->setmoveFlag();
		count++;
	}
	//XINPUT_STATE joyState;
	
	if (input->PushButtun(0, XINPUT_GAMEPAD_A)) {
		fade_->setmoveFlag();
	}
	if (sceneNum < 0) {
		sceneNum = 0;
		
	}
	fade_->Update();
	if (fade_->getchangeFlag()) {
		sceneNum = GAME_SCENE;
	}
	
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
	fade_->Draw();
}

void TitleScene::Finalize()
{
}
