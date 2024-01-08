#include "Enemy.h"
#include "ImguiManger.h"
void Enemy::Initialize(const std::vector<Model*>& models,Vector3 pos , int positionNo)
{
	ICharactor::Initialize(models,pos,positionNo);
	
	input_ = Input::GetInstance();
	
	//worldTransform_.Initialize();
	InitializeFloatGimmick();
	worldTransform_.translation_ = pos;
	worldTransform_.translation_.y = 5.0f;
	
	worldTransformBody_.translation_ = { pos.x,2.0f,pos.z };
	worldTransformHead_.translation_ = { 0.0f, 1.0f, 0.0f };
	worldTransformLarm_.translation_ = { -0.2f, 1.0f, 0.0f };
	worldTransformRarm_.translation_ = { 0.2f, 1.0f, 0.0f };
	worldTransform_.Initialize();
	worldTransformBase_.Initialize();
	worldTransformBody_.Initialize();
	worldTransformHead_.Initialize();
	worldTransformLarm_.Initialize();
	worldTransformRarm_.Initialize();
	SetParent(&GetWorldTransformBody());
	models_[kModelHead] = models[kModelHead];
	models_[kModelBody] = models[kModelBody];
	models_[kModelLarm] = models[kModelLarm];
	models_[kModelRarm] = models[kModelRarm];
	SetCollisionAttribute(CollisionConfig::kCollisionAttributeEnemy);
	SetCollisionMask(~CollisionConfig::kCollisionAttributeEnemy);
	move_ = { 1.0f,0.0f,0.0f };
	color = { 0.0f,0.0f,1.0f,1.0f };
	isAlive_ = true;
}

void Enemy::Update()
{
	if (isAlive_ == true) {
	//	structSphere_.center = worldTransformBody_.GetWorldPos();
	//	structSphere_.radius = 1.5f;
	//	//dateFloatGimmick();
	////Move();
	//	command_ = new CharaMoveCommand(this);

	//	command_->Exec();
		ModelUpdateMatrix();
		
	}
	
	models_[kModelBody]->SetColor(color);
	models_[kModelHead]->SetColor(color);
	models_[kModelLarm]->SetColor(color);
	models_[kModelRarm]->SetColor(color);
	
	
}





void Enemy::Draw(const ViewProjection& view)
{
	
	if (isAlive_ == true) {
		models_[kModelBody]->Draw(worldTransformBody_, view);
		models_[kModelHead]->Draw(worldTransformHead_, view);
		models_[kModelLarm]->Draw(worldTransformLarm_, view);
		models_[kModelRarm]->Draw(worldTransformRarm_, view);
	}
}

void Enemy::OnCollision()
{
	
}

void Enemy::isCollision()
{
	
	
}

void Enemy::Move(Vector3 velo)
{
	move_=velo;
	worldTransformBody_.translation_ = Add(worldTransformBody_.translation_, move_);
	/*if (worldTransformBody_.translation_.x >= 9.5f || worldTransformBody_.translation_.x <= -9.5) {
		move_ = Multiply(-1.0f, move_);
	}*/
}

void Enemy::Pass()
{
}

void Enemy::SetParent(const WorldTransform* parent)
{
	worldTransformBase_.parent_ = parent;
	worldTransformHead_.parent_ = parent;
	worldTransformRarm_.parent_ = parent;
	worldTransformLarm_.parent_ = parent;
	worldTransform_.parent_ = parent;
}

void Enemy::ModelUpdateMatrix()
{
	worldTransformBase_.UpdateMatrix();
	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformRarm_.UpdateMatrix();
	worldTransformLarm_.UpdateMatrix();
	
}

void Enemy::InitializeFloatGimmick()
{
	floatingParametor_ = 0.0f;
}

void Enemy::UpdateFloatGimmick()
{
	/*uint16_t T = 120;

	float step = 2.0f * (float)M_PI / T;
	float floatingAmplitude = 0.3f;

	floatingParametor_ += step;
	floatingParametor_ = (float)std::fmod(floatingParametor_, 2.0f * M_PI);



	worldTransformBody_.translation_.y = std::sin(floatingParametor_) * floatingAmplitude + 1.0f;

	worldTransformLarm_.rotation_.x = std::sin(floatingParametor_) * 0.75f;
	worldTransformRarm_.rotation_.x = std::sin(floatingParametor_) * 0.75f;*/
}


