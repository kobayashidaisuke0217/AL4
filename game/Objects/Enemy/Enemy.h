#pragma once
#include "Collider.h"
#include "CollisionConfig.h"
#include "WorldTransform.h"
#include "Model.h"
#include"Input.h"
#include"ViewProjection.h"
#include"ICharactor.h"
#include "../ground/../../Manager/ICommand.h"
#include "../../Manager/Ball.h"

class Enemy :public ICharactor, public Collider
{
public:
	void Initialize(const std::vector<Model*>& models,Vector3 pos, int positionNo) override;


	void Update()override;

	void Draw(const ViewProjection& view)override;
	void OnCollision() override;
	StructSphere GetStructSphere() { return structSphere_; }
	void isCollision();
	const WorldTransform& GetWorldTransformBody() { return worldTransformBody_; }
	WorldTransform GetWorldTransform()override { return worldTransformBase_; }
	bool GetisAlive() { return isAlive_; }
	void Move(Vector3 velo)override;
	void Pass()override;
	void SetTarget(const WorldTransform& trans) { target = trans; }
	OBB obb_;
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
	//Vector3 move_;
	StructSphere structSphere_;
	bool isAlive_=true;
	Vector4 color;
	ICommand* command_;
	Ball* ball;
	WorldTransform target;
private:
	
	void SetParent(const WorldTransform* parent);
	void ModelUpdateMatrix();
	void InitializeFloatGimmick();
	void UpdateFloatGimmick();
	
};