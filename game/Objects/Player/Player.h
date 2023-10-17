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
	void DeleteParent() { worldTransform_.parent_ = nullptr; worldTransform_.translation_ = worldTransform_.GetWorldPos(); }
private:
	Vector4 color;
	Input* input_ = nullptr;
	const ViewProjection* viewProjection_ = nullptr;
	Model* model_;
	StructSphere structSphere_;
	bool gameOver = false;
	WorldTransform worldTransform_;
	Matrix4x4 goal_;
	Matrix4x4 start_;
	Vector3 Translation_;
	bool MoveFlag = false;
	float moveSpeed;
	//!< 回転角度(90度の回転）
	float rad = 90 * 3.14f / 180;
	Quaternion quaternion_;
private:
	void Move();

};

