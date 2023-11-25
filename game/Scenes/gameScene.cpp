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
	  a = textureManager_->Load("Resource/tex.png");
	viewProjection_.Initialize();
	viewProjection_.translation_ = { 0.0f,0.0f,-5.0f };
	playerModel_.reset(Model::CreateModelFromObj("Resource", "saikoro.obj"));
	player_ = make_unique<Player>();
	enemyBodyModel.reset(Model::CreateModelFromObj("Resource", "float_Body.obj"));
	enemyHeadModel.reset(Model::CreateModelFromObj("Resource", "float_head.obj"));
	enemyL_armModel.reset(Model::CreateModelFromObj("Resource", "float_L_arm.obj"));
	enemyR_armModel.reset(Model::CreateModelFromObj("Resource", "float_R_arm.obj"));
	player_Hammer_.reset(Model::CreateModelFromObj("resource", "Hammer.obj"));
	std::vector<Model*>playerModels = { enemyBodyModel.get(),enemyHeadModel.get(),enemyL_armModel.get(),enemyR_armModel.get(),player_Hammer_.get() };
	player_->Initialize(playerModels,{10.0f,0.0f,0.0f});
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();
	followCamera_->SetTarget(&player_->GetWorldTransformBase());
	player_->SetViewProjection(&followCamera_->GetViewProjection());
	skyDomeModel_.reset(Model::CreateModelFromObj("Resource", "skyDome.obj"));
	skyDome_ = make_unique<SkyDome>();
	skyDome_->Initialize(skyDomeModel_.get());
	groundmanager_ = make_unique<groundManager>();
	groundmanager_->Initialize();
	goal_ = make_unique<Goal>();
	goal_->Initialize({ 0.0f,2.0f,62.0f }, { 1.0f,1.0f,1.0f });
	enemyModels = { enemyBodyModel.get(),enemyHeadModel.get(),enemyL_armModel.get(),enemyR_armModel.get() };
	enemys_.clear();
	for (int i = 0; i < 5; i++) {
		Enemy* enemy = new Enemy();
		enemy->Initialize(enemyModels, { i*1.0f,0.0f,i*10.0f });

		enemys_.push_back(enemy);
	}
	
	blendCount_ = 0;
	lockOn_ = make_unique<LookOn>();
	lockOn_->Initialize();
	player_->SetLockOn(lockOn_.get());
	followCamera_->SetlockOn(lockOn_.get());
	particle = make_unique<Particle>();
	particle->Initialize(1000);
	ApplyGlobalVariables();
	count_ = 0;
}

void GameScene::Update()
{
	count_++;
	groundmanager_->Update();
	for (std::list<Enemy*>::iterator enemy = enemys_.begin(); enemy != enemys_.end(); enemy++) {
		if ((*enemy)->GetisAlive()) {
			(*enemy)->Update();
		}
	}
	if (player_->isGameover() == true) {
		Finalize();
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

		/*if (IsCollision(groundmanager_->GetOBB(2), player_->GetStructSphere())) {
			player_->isHit_ = true;
			player_->IsCollision(groundmanager_->GetMoveGround()->GetWorldTransform());
		}
		else {
			player_->DeleteParent();
		}*/


		if (player_->GetIsAtack()) {
			int i = 0;
				for (Enemy* enemy : enemys_) {
					if (enemy->GetisAlive()) {
						if (IsCollision(player_->getcollsionObb(), enemy->GetStructSphere())) {
							enemy->IsDead();
							Transform t = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},enemy->GetWorldTransformBody().translation_};
							particle->AddParticle({t}, 35);
							i++;
						}
					}
				}
			}
		
		player_->Update();
		viewProjection_.UpdateMatrix();
		followCamera_->Update();
		viewProjection_.matView = followCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = followCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
		lockOn_->Update(enemys_, viewProjection_);
		particle->Update();
		if (sceneNum > 1) {
			sceneNum = 1;
		}


		collisionManager_->ClearColliders();
		collisionManager_->AddCollider(player_.get());
		collisionManager_->AddCollider(goal_.get());
		/*enemys_.remove_if([](Enemy* enemy) {
			if (!enemy->GetisAlive()) {
				delete enemy;
				return true;
			}
			return false;
			});*/
		for (std::list<Enemy*>::iterator enemy = enemys_.begin(); enemy != enemys_.end(); enemy++) {

			if ((*enemy)) {
				if ((*enemy)->GetisAlive()) {
					collisionManager_->AddCollider((*enemy));
				}
			}
		}
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
	for (std::list<Enemy*>::iterator enemy = enemys_.begin(); enemy != enemys_.end(); enemy++) {
		(*enemy)->Draw(viewProjection_);
	}
	blueMoon_->PariclePreDraw();
    particle->Draw(viewProjection_,{1.0f,1.0f,1.0f,1.0f},a);

}

void GameScene::ApplyGlobalVariables()
{
	GlovalVariables* globalVariables = GlovalVariables::GetInstance();

	const char* groupName = "Player";

}

void GameScene::Draw2D() {
	blueMoon_->SetBlendMode(kBlendModeNormal);
	lockOn_->Draw();

}
void GameScene::Finalize()
{



}