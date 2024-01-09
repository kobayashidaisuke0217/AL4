#include "gameScene.h"


GameScene::~GameScene()
{

}

void GameScene::Initialize()
{
	blueMoon_ = BlueMoon::GetInstance();

	directXCommon_ = DirectXCommon::GetInstance();
	map_=MoveMap:: GetInstance();
	map_->Init();
	
	textureManager_ = Texturemanager::GetInstance();
	collisionManager_ = make_unique<CollisionManager>();
	input_ = Input::GetInstance();
	  a = textureManager_->Load("Resource/circle.png");
	viewProjection_.Initialize();
	viewProjection_.translation_ = { 0.0f,0.0f,-5.0f };
	playerModel_.reset(Model::CreateModelFromObj("Resource", "saikoro.obj"));

	ball_ = Ball::GetInstance();
	ball_->Init();
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

	followCamera_ = std::make_unique<FollowCamera>();
	player_ = make_unique<PlayerManager>();
	player_->Initialize(playerModels);
	followCamera_->Initialize();



	skyDomeModel_.reset(Model::CreateModelFromObj("Resource", "skyDome.obj"));
	skyDome_ = make_unique<SkyDome>();
	skyDome_->Initialize(skyDomeModel_.get());
	groundmanager_ = make_unique<groundManager>();
	groundmanager_->Initialize();
	enemyModels = { enemyBodyModel.get(),enemyHeadModel.get(),enemyL_armModel.get(),enemyR_armModel.get() };
	enemys_.clear();
	goal_ = std::make_unique<Goal>();
	goal_->Init();
	Enemy* enemy[1];
	for (int i = 0; i < 1; i++) {
		enemy[i] = new Enemy();
	}
	enemy[0]->Initialize(enemyModels, { 10.0f ,1.0f, 10.0f }, GoalKeeper);
	enemy[0]->SetTarget(goal_->transform_);
	enemys_.push_back(enemy[0]);

	uiSprite_ = new Sprite();
	uiSprite_->Initialize(textureManager_->Load("Resource/ui.png"));


	blendCount_ = 0;

	particle = make_unique<Particle>();
	particle->Initialize(1000);

	count_ = 0;
	GlovalVariables* globalVariables = GlovalVariables::GetInstance();

	const char* groupName = "view";
	globalVariables->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "transration", viewProjection_.translation_);
	globalVariables->AddItem(groupName, "Rotate", viewProjection_.rotation_);

	goalCount_ = 0;
	isGoal_ = false;ApplyGlobalVariables();
}

void GameScene::Update()
{
	uiSprite_->position = { 640.0f,360.0f };
	uiSprite_->size_ = { 35.0f,26.0f };
	count_++;
	groundmanager_->Update();
	ball_->Update();
	goal_->Update();
	ImGui::Begin("Scene");
	ImGui::DragFloat3("translate", &uiSprite_->position.x, 0.1f);
	ImGui::DragFloat3("scale", &uiSprite_->size_.x, 0.1f);
	ImGui::End();
	if (count_ >= 5) {


		/*if (IsCollision(groundmanager_->GetOBB(2), player_->GetStructSphere())) {
			player_->isHit_ = true;
			player_->IsCollision(groundmanager_->GetMoveGround()->GetWorldTransform());
		}
		else {
			player_->DeleteParent();
		}*/
		if (!isGoal_) {
			player_->Update();

			for (std::list<Enemy*>::iterator enemy = enemys_.begin(); enemy != enemys_.end(); enemy++) {
				if ((*enemy)->GetisAlive()) {
					(*enemy)->Update();
				}
			}
		}

		viewProjection_.UpdateMatrix();

		viewProjection_.TransferMatrix();

		particle->Update();
		if (sceneNum > 1) {
			sceneNum = 1;
		}


		collisionManager_->ClearColliders();
		if (!isGoal_) {
			for (std::list<Enemy*>::iterator enemy = enemys_.begin(); enemy != enemys_.end(); enemy++) {
				
			
				if (IsCollision((*enemy)->obb_, ball_->structSphere_)) {
					ball_->Init();
					return;
				}
			}
		}
		if (IsCollision(goal_->obb_, ball_->structSphere_)) {
			isGoal_ = true;
			ball_->velocity_ = { 0.0f,0.0f,0.0f };
			particle->AddParticle({goal_->transform_.scale_,{1.0f,1.0f,1.0f}, goal_->transform_.translation_}, 25);
			viewProjection_.translation_ = { -36.1f,36.1f,17.7f };
			viewProjection_.rotation_ = { 0.5f,1.6f,0.0f };
		}
		if (isGoal_) {
			goalCount_++;
		}
		if (goalCount_ >= 60) {
			sceneNum = TITLE_SCENE;
		}
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
	//skyDome_->Draw(viewProjection_);
	
	groundmanager_->Draw(viewProjection_);
	player_->Draw(viewProjection_);
	for (std::list<Enemy*>::iterator enemy = enemys_.begin(); enemy != enemys_.end(); enemy++) {
		(*enemy)->Draw(viewProjection_);
	}
	goal_->Draw(viewProjection_);
	ball_->Draw(viewProjection_);
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
	uiSprite_->Draw({
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
		},{1.0f,1.0f,1.0f,1.0f});

}
void GameScene::Finalize()
{



}