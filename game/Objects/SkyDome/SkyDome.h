#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
class SkyDome {
public:
	void Initialize(Model* model);

	void Update();

	void Draw(const ViewProjection& viewPrijection);

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
};