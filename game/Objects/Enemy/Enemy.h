#pragma once
#include "Collider.h"
#include "CollisionConfig.h"
#include "WorldTransform.h"
#include "Model.h"
#include"Input.h"
#include"ViewProjection.h"
#include"ICharactor.h"
class Enemy :public ICharactor, public Collider
{
public:
	void Initialize(const std::vector<Model*>& models,Vector3 pos) override;


	void Update()override;
	StructSphere GetStructSphere() { return structSphere_; }
	void Draw(const ViewProjection& view)override;
	void OnCollision() override;
	void isCollision();
	const WorldTransform& GetWorldTransformBody() { return worldTransformBody_; }
	WorldTransform GetWorldTransform()override { return worldTransformBase_; }
	void IsDead();
	bool GetisAlive() { return isAlive_; }
	void resetHP() { HP = 3; }
	void SetVelocity(Vector3 a) { knockBacvelo = a; }
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
	Vector3 move_;
	StructSphere structSphere_;
	bool isAlive_=true;
	int32_t HP = 3;
	Vector4 color;
	Vector3 knockBacvelo;
	bool knockback;
private:
	void Move();
	void SetParent(const WorldTransform* parent);
	void ModelUpdateMatrix();
	void InitializeFloatGimmick();
	void UpdateFloatGimmick();
	void KnockBack();
};