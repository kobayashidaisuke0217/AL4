#pragma once
#include "model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
class Goal
{
public:
	void Init();
	void Update();
	void Draw(ViewProjection& view);
	OBB obb_;
public:
	WorldTransform transform_;
	
	std::unique_ptr<Model> model_;
};

