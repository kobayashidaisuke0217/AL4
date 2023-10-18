#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"
#include "ViewProjection.h"
#include "Collider.h"
#include "CollisionConfig.h"
class Player:public Collider
{
public:
	void Initialize( Model* model);
	void Update();
	void Draw(const ViewProjection& view);
    WorldTransform GetWorldTransform()override { return worldTransform_; }
	const WorldTransform& GetWorldTransformBase() { return worldTransform_; }
	void SetViewProjection(const ViewProjection* view) { viewProjection_ = view; }
	void IsFall();
	StructSphere GetStructSphere() { return structSphere_; }
	bool isHit_;
	bool isGameover() { return gameOver; }
	void OnCollision() override;
	void Setparent(const WorldTransform* parent);
	void IsCollision(const WorldTransform& worldtransform );
	void DeleteParent();
	void SetObjectPos(const WorldTransform& worldtransform) { objectPos_ = worldtransform; }
private:
	Vector4 color;
	Input* input_ = nullptr;
	const ViewProjection* viewProjection_ = nullptr;
	Model* model_;
	StructSphere structSphere_;
	bool gameOver = false;
	WorldTransform worldTransform_;
	WorldTransform objectPos_;
private:
	void Move();

};

