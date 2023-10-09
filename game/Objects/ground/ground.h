#pragma once
#include "Model.h"
#include "worldTransform.h"
#include "ImguiManger.h"
#include <memory>
#include "MyMath.h"
#include "ViewProjection.h"
#include "textureManager.h"
class ground
{
public:
	void Initialize(Model*model,Vector3 translation,Vector3 Scale);
	void Update();
	void Draw(const ViewProjection&view);
	WorldTransform& GetWorldTransform() { return worldTtansform_; }
private:
	Texturemanager* texturemanager_;
	WorldTransform worldTtansform_;
	Model* groundModel_;
	uint32_t texhandle_;
};

