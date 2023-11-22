#include "ICharactor.h"

void ICharactor::Initialize(const std::vector<Model*>& models,Vector3 pos)
{
	models_ = models;
	worldTransform_.translation_ = pos;
	worldTransform_.Initialize();
}

void ICharactor::Update()
{
	worldTransform_.UpdateMatrix();
}

void ICharactor::Draw(const ViewProjection& view)
{
	for (Model* model : models_) {
		model->Draw(worldTransform_, view);
	}
}
