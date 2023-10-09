#include "ICharactor.h"

void ICharactor::Initialize(const std::vector<Model*>& models)
{
	models_ = models;
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
