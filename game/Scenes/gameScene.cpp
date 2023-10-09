#include "gameScene.h"


GameScene::~GameScene()
{
	
}

void GameScene::Initialize()
{
	blueMoon_ = BlueMoon::GetInstance();

	directXCommon_ = DirectXCommon::GetInstance();

	textureManager_ = Texturemanager::GetInstance();
	input_ = Input::GetInstance();
	int a= textureManager_->Load("Resource/tex.png");
	viewProjection_.Initialize();
	viewProjection_.translation_ = { 0.0f,0.0f,-5.0f };
	playerModel_.reset(Model::CreateModelFromObj("Resource", "Cube.obj"));
	player_ = make_unique<Player>();
	player_->Initialize(playerModel_.get());
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();
	followCamera_->SetTarget(&player_->GetWorldTransform());
	player_->SetViewProjection(&followCamera_->GetViewProjection());
	skyDomeModel_ .reset( Model::CreateModelFromObj("Resource","skyDome.obj"));
	skyDome_ = make_unique<SkyDome>();
	skyDome_->Initialize(skyDomeModel_.get());
	groundmanager_ = make_unique<groundManager>();
	groundmanager_->Initialize();

	GlovalVariables* globalVariables{};
	globalVariables = GlovalVariables::GetInstance();
	blendCount_ = 0;
	const char* groupName = "Player";
	GlovalVariables::GetInstance()->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "Test", 90.0f);
	ApplyGlobalVariables();
	
}

void GameScene::Update()
{
	player_->Update();
	if (player_->isGameover() == true) {
		Initialize();
	}
	player_->isHit_ = false;
	groundmanager_->Update();
	for (int i = 0; i < 2; i++) {
		if (IsCollision(groundmanager_->GetOBB(i), player_->GetStructSphere())) {
			player_->isHit_ = true;
		}
	}
	
	viewProjection_.UpdateMatrix();
	followCamera_->Update();
	viewProjection_.matView = followCamera_->GetViewProjection().matView;
	viewProjection_.matProjection = followCamera_->GetViewProjection().matProjection;
	viewProjection_.TransferMatrix();
	
	ImGui::Begin("Scene");
	
	ImGui::InputInt("blendCount", &blendCount_);
	ImGui::InputFloat3("rotate", &viewProjection_.rotation_.x);
	ImGui::InputInt("SceneNum", &sceneNum);
	if (sceneNum > 1) {
		sceneNum = 1;
	}
	ImGui::End();
}


void GameScene::Draw()
{
	
	//3D描画準備
	blueMoon_->ModelPreDraw();
	Draw3D();
	//2D描画準備
	blueMoon_->SpritePreDraw();
	Draw2D();
}

void GameScene::Draw3D()
{
	skyDome_->Draw(viewProjection_);
	player_->Draw(viewProjection_);
	groundmanager_->Draw(viewProjection_);
}

void GameScene::ApplyGlobalVariables()
{
	GlovalVariables* globalVariables = GlovalVariables::GetInstance();

	const char* groupName = "Player";
	
}

void GameScene::Draw2D() {
	blueMoon_->SetBlendMode(blendCount_);
	

}
void GameScene::Finalize()
{
	
	
	
}

