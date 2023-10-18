#include "Enemy.h"
#include "ImguiManger.h"
void Enemy::Initialize(const std::vector<Model*>& models)
{
	ICharactor::Initialize(models);
	models_[kModelHead] = models[kModelHead];
	models_[kModelBody] = models[kModelBody];
	models_[kModelLarm] = models[kModelLarm];
	models_[kModelRarm] = models[kModelRarm];
	input_ = Input::GetInstance();
	
	//worldTransform_.Initialize();
	InitializeFloatGimmick();

	worldTransform_.translation_.y = 5.0f;
	worldTransformBody_.translation_ = { 0.0f,2.0f,55.0f };
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
	move_ = { 0.3f,0.0f,0.0f };
}

void Enemy::Update()
{
	UpdateFloatGimmick();
	Move();
	ModelUpdateMatrix();
	ImGui::Begin("Enemy");
	ImGui::DragFloat3("body", &worldTransformBody_.translation_.x, 0.1f);
	ImGui::End();
}





void Enemy::Draw(const ViewProjection& view)
{
	models_[kModelBody]->Draw(worldTransformBody_, view);
	models_[kModelHead]->Draw(worldTransformHead_, view);
	models_[kModelLarm]->Draw(worldTransformLarm_, view);
	models_[kModelRarm]->Draw(worldTransformRarm_, view);
	
}

void Enemy::OnCollision()
{
}

void Enemy::Move()
{
	worldTransformBody_.translation_ = Add(worldTransformBody_.translation_, move_);
	if (worldTransformBody_.translation_.x >= 9.5f || worldTransformBody_.translation_.x <= -9.5) {
		move_ = Multiply(-1.0f, move_);
	}
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
	uint16_t T = 120;

	float step = 2.0f * (float)M_PI / T;
	float floatingAmplitude = 0.3f;

	floatingParametor_ += step;
	floatingParametor_ = (float)std::fmod(floatingParametor_, 2.0f * M_PI);



	worldTransformBody_.translation_.y = std::sin(floatingParametor_) * floatingAmplitude + 1.0f;

	worldTransformLarm_.rotation_.x = std::sin(floatingParametor_) * 0.75f;
	worldTransformRarm_.rotation_.x = std::sin(floatingParametor_) * 0.75f;
}
