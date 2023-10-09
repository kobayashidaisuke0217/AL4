#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <Model.h>
#include <vector>
enum ModelParts { kModelBody, kModelHead, kModelLarm, kModelRarm, kModelHammer };
class ICharactor
{
public:
	virtual void Initialize(const std::vector<Model*>& models);

	virtual void Update();

	virtual void Draw(const ViewProjection& view);



protected:
	std::vector<Model*> models_;
	WorldTransform worldTransform_;
};

