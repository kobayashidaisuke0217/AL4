#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <Model.h>
#include <vector>
enum ModelParts { kModelBody, kModelHead, kModelLarm, kModelRarm };
class ICharactor
{
public:
	virtual void Initialize(const std::vector<Model*>& models,Vector3 pos,int positionNo);

	virtual void Update();
	virtual void Pass()=0;
	virtual void Draw(const ViewProjection& view);
	virtual void Move(Vector3 velo)=0;
	virtual Vector3 GetVelocity() { return move_; }
protected:
	std::vector<Model*> models_;
	WorldTransform worldTransform_;
	Vector3 move_;
};

