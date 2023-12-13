#pragma once
#include "Collider.h"
#include "CollisionConfig.h"
#include "WorldTransform.h"
#include "Model.h"
#include"Input.h"
#include"ViewProjection.h"
#include"ICharactor.h"
class Player:public ICharactor
{
public:
	void Initialize(const std::vector<Model*>& models, Vector3 pos) override;


	void Update()override;

	void Draw(const ViewProjection& view)override;
	
	StructSphere GetStructSphere() { return structSphere_; }
	void isCollision();
	const WorldTransform& GetWorldTransformBody() { return worldTransformBody_; }
	WorldTransform GetWorldTransform() { return worldTransformBase_; }
	bool GetisAlive() { return isAlive_; }

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
	bool isAlive_ = true;
	Vector4 color;
	bool isMove_;
	float moveSpeed_;
	Quaternion quaternion_;
	Vector3 preMove_;
	Quaternion preQuaternion_;
	Vector3 MoveVec;
	bool isSelected;
private:
	void MoveAI();
	void MovePlay();
	void SetParent(const WorldTransform* parent);
	void ModelUpdateMatrix();
	void InitializeFloatGimmick();
	void UpdateFloatGimmick();

};

