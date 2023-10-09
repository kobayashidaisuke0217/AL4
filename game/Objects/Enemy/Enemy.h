#pragma once
#include "Collider.h"
#include "CollisionConfig.h"
#include "WorldTransform.h"
#include "Model.h"
#include"Input.h"
#include"ViewProjection.h"
#include"ICharactor.h"
class Enemy:public ICharactor,public Collider
{
public:
	void Initialize(const std::vector<Model*>& models) override;


	void Update()override;

	void Draw(const ViewProjection& view)override;
	void OnCollision() override;
	const WorldTransform& GetWorldTransformBody() { return worldTransformBody_; }
 WorldTransform GetWorldTransform()override { return worldTransformBase_; }
private:
	WorldTransform worldTransformBase_;
	WorldTransform worldTransformHead_;
	WorldTransform worldTransformLarm_;
	WorldTransform worldTransformRarm_;
	WorldTransform worldTransformBody_;
	Input* input_ = nullptr;
	const ViewProjection* viewProjection_ = nullptr;
	float floatingParametor_ = 0.0f;
	int animationFrame;
private:
	void Move();
	void SetParent(const WorldTransform* parent);
	void ModelUpdateMatrix();
	void InitializeFloatGimmick();
	void UpdateFloatGimmick();
};

