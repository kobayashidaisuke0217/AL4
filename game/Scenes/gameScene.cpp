#include "gameScene.h"


GameScene::~GameScene()
{
	
}

void GameScene::Initialize()
{
	blueMoon_ = BlueMoon::GetInstance();

	directXCommon_ = DirectXCommon::GetInstance();

	textureManager_ = Texturemanager::GetInstance();
	collisionManager_ = make_unique<CollisionManager>();
	input_ = Input::GetInstance();
	int a= textureManager_->Load("Resource/tex.png");
	viewProjection_.Initialize();
	viewProjection_.translation_ = { 0.0f,0.0f,-5.0f };
	playerModel_.reset(Model::CreateModelFromObj("Resource", "saikoro.obj"));
	player_ = make_unique<Player>();
	enemyBodyModel.reset(Model::CreateModelFromObj("Resource", "float_Body.obj"));
	enemyHeadModel.reset(Model::CreateModelFromObj("Resource", "float_head.obj"));
	enemyL_armModel.reset(Model::CreateModelFromObj("Resource", "float_L_arm.obj"));
	enemyR_armModel.reset(Model::CreateModelFromObj("Resource", "float_R_arm.obj"));
	player_Hammer_.reset(Model::CreateModelFromObj("resource", "Hammer.obj"));
	std::vector<Model*>playerModels = { enemyBodyModel.get(),enemyHeadModel.get(),enemyL_armModel.get(),enemyR_armModel.get(),player_Hammer_.get()};
	player_->Initialize(playerModels);
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();
	followCamera_->SetTarget(&player_->GetWorldTransformBase());
	player_->SetViewProjection(&followCamera_->GetViewProjection());
	skyDomeModel_ .reset( Model::CreateModelFromObj("Resource","skyDome.obj"));
	skyDome_ = make_unique<SkyDome>();
	skyDome_->Initialize(skyDomeModel_.get());
	groundmanager_ = make_unique<groundManager>();
	groundmanager_->Initialize();
	goal_ = make_unique<Goal>();
	goal_->Initialize({0.0f,2.0f,62.0f},{1.0f,1.0f,1.0f});
	enemy_ = make_unique<Enemy>();
	
	std::vector<Model*>enemyModels = { enemyBodyModel.get(),enemyHeadModel.get(),enemyL_armModel.get(),enemyR_armModel.get() };
	enemy_->Initialize(enemyModels);
	blendCount_ = 0;
	
	ApplyGlobalVariables();
	count_ = 0;
}

void GameScene::Update()
{
	count_++;
	groundmanager_->Update();
	
	enemy_->Update();
	if (player_->isGameover() == true) {
		Initialize();
	}
	player_->isHit_ = false;

	goal_->Update();
	if (count_ >= 5) {
		for (int i = 0; i < 2; i++) {
			if (IsCollision(groundmanager_->GetOBB(i), player_->GetStructSphere())) {
				player_->isHit_ = true;
				player_->SetObjectPos(groundmanager_->GetGround(i)->GetWorldTransform());
			}
		}

		if (IsCollision(groundmanager_->GetOBB(2), player_->GetStructSphere())) {
			player_->isHit_ = true;
			player_->IsCollision(groundmanager_->GetMoveGround()->GetWorldTransform());
		}
		else {
			player_->DeleteParent();
		}



		if (IsCollision(player_->getcollsionObb(), enemy_->GetStructSphere())) {
			enemy_->IsDead();
		}
	}
	player_->Update();
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

	collisionManager_->ClearColliders();
	collisionManager_->AddCollider(player_.get());
	collisionManager_->AddCollider(goal_.get());
	if (enemy_) {
		collisionManager_->AddCollider(enemy_.get());
	}
	if (count_ >= 20) {
		collisionManager_->CheckAllCollision();
	}
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
	goal_->Draw(viewProjection_);
	enemy_->Draw(viewProjection_);
	
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

