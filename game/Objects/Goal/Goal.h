#pragma once
#include "Collider.h"
#include "CollisionConfig.h"
#include "WorldTransform.h"
#include "Sphere.h"
#include "ViewProjection.h"
class Goal :public Collider
{
public:
	void Initialize(Vector3 transform, Vector3 scale);
	void Update();
	void Draw(const ViewProjection& view);
	void OnCollision() override;
	WorldTransform GetWorldTransform()override { return worldTransform_; }
private:
	WorldTransform worldTransform_;
	std::unique_ptr<Sphere> sphere_;
};