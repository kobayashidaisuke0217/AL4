#include "groundManager.h"

void groundManager::Initialize()
{
	for (int i = 0; i < 2; i++) {
		ground_[i] = make_unique<ground>();
	
	}
	moveGround_ = make_unique<MoveGround>();
	movemodel_.reset(Model::CreateModelFromObj("Resource", "move.obj"));
	model_.reset(Model::CreateModelFromObj("Resource", "CUBE.obj"));
	ground_[0]->Initialize(model_.get(), { 0.0f,0.0f,-5.0f }, { 90.0f,1.0f,17.0f });
	ground_[1]->Initialize(model_.get(), { 0.0f,0.0f,55.0f }, {90.0f,1.0f,17.0f });
	moveGround_->Initialize(movemodel_.get(), { 0.0f,0.0f,25.0f }, { 1.0f,1.0f,1.0f });
	
}

void groundManager::Update()
{
	for (int i = 0; i < 2; i++) {
		Obb_[i].center = ground_[i]->GetWorldTransform().GetWorldPos();
		GetOrientations(MakeRotateXYZMatrix(ground_[i]->GetWorldTransform().rotation_), Obb_[i].orientation);
		Obb_[i].size = ground_[i]->GetWorldTransform().scale_;
	}
	Obb_[2].center = moveGround_->GetWorldTransform().GetWorldPos();
	GetOrientations(MakeRotateXYZMatrix(moveGround_->GetWorldTransform().rotation_), Obb_[2].orientation);
	Obb_[2].size = moveGround_->GetWorldTransform().scale_;
	
	for (int i = 0; i < 2; i++) {
		ground_[i]->Update();

	}
	moveGround_->Update();
}

void groundManager::Draw(const ViewProjection& view)
{
	for (int i = 0; i < 2; i++) {
		ground_[i]->Draw(view);

	}

	moveGround_->Draw(view);
}
