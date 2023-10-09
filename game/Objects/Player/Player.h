#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include"Input.h"
#include"ViewProjection.h"
class Player
{
public:
	void Initialize( Model* model);
	void Update();
	void Draw(const ViewProjection& view);
	const WorldTransform& GetWorldTransform() { return worldTransform_; }
	void SetViewProjection(const ViewProjection* view) { viewProjection_ = view; }
private:
	WorldTransform worldTransform_;
	Input* input_ = nullptr;
	const ViewProjection* viewProjection_ = nullptr;
	Model* model_;
private:
	void Move();
};

