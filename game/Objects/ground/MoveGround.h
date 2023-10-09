#pragma once
#include "Model.h"
#include "worldTransform.h"
#include "ImguiManger.h"
#include <memory>
#include "MyMath.h"
#include "ViewProjection.h"
#include "textureManager.h"
class MoveGround
{
public:
	void Initialize(Model* model, Vector3 translation, Vector3 Scale);
	void Update();
	void Draw(const ViewProjection& view);
	WorldTransform& GetWorldTransform() { return worldTransform_; }
	WorldTransform& GetWorldTransformBase_() { return worldTransformBase_; }
private:
	WorldTransform worldTransformBase_;
	Texturemanager* texturemanager_;
	WorldTransform worldTransform_;
	Model* groundModel_;
	Vector3 move_;
private:
	void SetParent(const WorldTransform* parent);
	void Move();
};

