#include "SkyDome.h"
#include <cassert>
void SkyDome::Initialize(Model* model) {
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.scale_ = { 100.0f, 100.0f, 100.0f };
	worldTransform_.UpdateMatrix();
}

void SkyDome::Update() {
	worldTransform_.UpdateMatrix();

}

void SkyDome::Draw(const ViewProjection& viewPrijection) {
	model_->Draw(worldTransform_, viewPrijection);
}