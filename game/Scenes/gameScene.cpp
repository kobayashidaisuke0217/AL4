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

	enemyBodyModel.reset(Model::CreateModelFromObj("Resource", "float_Body.obj"));
	enemyHeadModel.reset(Model::CreateModelFromObj("Resource", "float_head.obj"));
	enemyL_armModel.reset(Model::CreateModelFromObj("Resource", "float_L_arm.obj"));
	enemyR_armModel.reset(Model::CreateModelFromObj("Resource", "float_R_arm.obj"));
	BodyModel.reset(Model::CreateModelFromObj("Resource", "float_Body.obj"));
	HeadModel.reset(Model::CreateModelFromObj("Resource", "float_head.obj"));
	L_armModel.reset(Model::CreateModelFromObj("Resource", "float_L_arm.obj"));
	R_armModel.reset(Model::CreateModelFromObj("Resource", "float_R_arm.obj"));
	player_Hammer_.reset(Model::CreateModelFromObj("resource", "Hammer.obj"));
	std::vector<Model*>playerModels = { BodyModel.get(),HeadModel.get(),L_armModel.get(),R_armModel.get() };

	followCamera_ = std::make_unique<FollowCamera>(); player_ = make_unique<Player>();
	player_->Initialize(playerModels, { 10.0f,0.0f,0.0f });
	followCamera_->Initialize();

	
	skyDomeModel_.reset(Model::CreateModelFromObj("Resource", "skyDome.obj"));
	skyDome_ = make_unique<SkyDome>();
	skyDome_->Initialize(skyDomeModel_.get());
	groundmanager_ = make_unique<groundManager>();
	groundmanager_->Initialize();
	enemyModels = { enemyBodyModel.get(),enemyHeadModel.get(),enemyL_armModel.get(),enemyR_armModel.get() };
	enemys_.clear();
	Enemy* enemy[5];
	for (int i = 0; i < 5; i++) {
		enemy[i] = new Enemy();
	}
	enemy[0]->Initialize(enemyModels, {10.0f ,1.0f, 10.0f});

	enemys_.push_back(enemy[0]);
	
	enemy[1]->Initialize(enemyModels, {20.0f ,1.0f, 20.0f});

	enemys_.push_back(enemy[1]);

	enemy[2]->Initialize(enemyModels, {-10.0f ,1.0f, -10.0f});

	enemys_.push_back(enemy[2]);
	enemy[3]->Initialize(enemyModels, {-20.0f ,1.0f, -20.0f});

	enemys_.push_back(enemy[3]);
	enemy[4]->Initialize(enemyModels, {20.0f ,1.0f, 0.0f});

	enemys_.push_back(enemy[4]);
	blendCount_ = 0;
	
	particle = make_unique<Particle>();
	particle->Initialize(1000);
	
	count_ = 0;
	GlovalVariables* globalVariables = GlovalVariables::GetInstance();

	const char* groupName = "view";
	globalVariables->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "transration", viewProjection_.translation_);
	globalVariables->AddItem(groupName, "Rotate", viewProjection_.rotation_);


}

void GameScene::Update()
{
	ApplyGlobalVariables();
	count_++;
	groundmanager_->Update();
	
	
		
	
	if (count_ >= 5) {
		

		/*if (IsCollision(groundmanager_->GetOBB(2), player_->GetStructSphere())) {
			player_->isHit_ = true;
			player_->IsCollision(groundmanager_->GetMoveGround()->GetWorldTransform());
		}
		else {
			player_->DeleteParent();
		}*/

		player_->Update();
	
		for (std::list<Enemy*>::iterator enemy = enemys_.begin(); enemy != enemys_.end(); enemy++) {
			if ((*enemy)->GetisAlive()) {
				(*enemy)->Update();
			}
		}
		
		viewProjection_.UpdateMatrix();
	
		viewProjection_.TransferMatrix();
	
		particle->Update();
		if (sceneNum > 1) {
			sceneNum = 1;
		}


		collisionManager_->ClearColliders();
		
		
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
	
	groundmanager_->Draw(viewProjection_);
	player_->Draw(viewProjection_);
	for (std::list<Enemy*>::iterator enemy = enemys_.begin(); enemy != enemys_.end(); enemy++) {
		(*enemy)->Draw(viewProjection_);
	}
	blueMoon_->PariclePreDraw();
    particle->Draw(viewProjection_,{1.0f,1.0f,1.0f,1.0f},a);

}

void GameScene::ApplyGlobalVariables()
{
	GlovalVariables* globalVariables = GlovalVariables::GetInstance();

	const char* groupName = "view";
	viewProjection_.rotation_ = globalVariables->GetVector3Value(groupName, "Rotate");
	viewProjection_.translation_ = globalVariables->GetVector3Value(groupName, "transration");
}

void GameScene::Draw2D() {
	blueMoon_->SetBlendMode(kBlendModeNormal);
	

}
void GameScene::Finalize()
{



}