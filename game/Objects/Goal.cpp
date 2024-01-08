#include "Goal.h"
#include "Imguimanger.h"
void Goal::Init()
{
	transform_.Initialize();
	transform_.translation_.x = 77.0f;
	transform_.translation_.z = 24.0f;
	transform_.scale_ = { 1.0f,6.0f,8.0f };
	model_.reset(Model::CreateModelFromObj("Resource", "move.obj"));
}

void Goal::Update()
{
	obb_.center = transform_.GetWorldPos();//worldTransformHammer_.GetWorldPos();

	GetOrientations(MakeRotateMatrix({ 0.0f,0.0f,0.0f }), obb_.orientation);
	obb_.size = transform_.scale_;
	ImGui::Begin("goal");
	ImGui::DragFloat3("trans", &transform_.translation_.x);
	ImGui::DragFloat3("scale", &transform_.scale_.x);
	ImGui::End();
	transform_.UpdateMatrix();
}

void Goal::Draw(ViewProjection& view)
{
	model_->Draw(transform_, view);
}
