#include "groundManager.h"

void groundManager::Initialize()
{
	for (int i = 0; i < 2; i++) {
		ground_[i] = make_unique<ground>();
	groundList_.push_back(ground_[i].get());
	}
	moveGround_ = make_unique<MoveGround>();
	movemodel_.reset(Model::CreateModelFromObj("Resource", "move.obj"));
	model_.reset(Model::CreateModelFromObj("Resource", "CUBE.obj"));
	ground_[0]->Initialize(model_.get(), { 0.0f,-1.5f,-5.0f }, { 5.0f,0.5f,17.0f });
	ground_[1]->Initialize(model_.get(), { 0.0f,-1.5f,55.0f }, { 10.0f,0.5f,17.0f });
	moveGround_->Initialize(movemodel_.get(), { 0.0f,-1.5f,25.0f }, { 5.0f,0.5f,4.0f });
	
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
	ImGui::Begin("Plane");
	ImGui::DragFloat3("scale", &moveGround_->GetWorldTransform().scale_.x);
	ImGui::DragFloat3("rotate", &moveGround_->GetWorldTransform().rotation_.x);
	ImGui::DragFloat3("translate", &moveGround_->GetWorldTransform().translation_.x);
	ImGui::End();
	for (ground* Ground : groundList_) {
		Ground->Update();
	}
	moveGround_->Update();
}

void groundManager::Draw(const ViewProjection& view)
{
	for (ground* Ground : groundList_) {
		Ground->Draw(view);
	}
	moveGround_->Draw(view);
}
