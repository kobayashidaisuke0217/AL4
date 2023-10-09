#include "groundManager.h"

void groundManager::Initialize()
{
	for (int i = 0; i < 2; i++) {
		ground_[i] = make_unique<ground>();
	groundList_.push_back(ground_[i].get());
	}
	model_.reset(Model::CreateModelFromObj("Resource", "Cube.obj"));
	ground_[0]->Initialize(model_.get(), { 0.0f,-1.5f,1.0f }, { 5.0f,0.5f,17.0f });
	ground_[1]->Initialize(model_.get(), { 0.0f,-1.5f,55.0f }, { 5.0f,0.5f,17.0f });
	
	
}

void groundManager::Update()
{
	for (int i = 0; i < 2; i++) {
		Obb_[i].center = ground_[i]->GetWorldTransform().GetWorldPos();
		GetOrientations(MakeRotateXYZMatrix(ground_[i]->GetWorldTransform().rotation_), Obb_[i].orientation);
		Obb_[i].size = ground_[i]->GetWorldTransform().scale_;
	}
	ImGui::Begin("Plane");
	ImGui::DragFloat3("scale", &ground_[1]->GetWorldTransform().scale_.x);
	ImGui::DragFloat3("rotate", &ground_[1]->GetWorldTransform().rotation_.x);
	ImGui::DragFloat3("translate", &ground_[1]->GetWorldTransform().translation_.x);
	ImGui::End();
	for (ground* Ground : groundList_) {
		Ground->Update();
	}
}

void groundManager::Draw(const ViewProjection& view)
{
	for (ground* Ground : groundList_) {
		Ground->Draw(view);
	}
}
