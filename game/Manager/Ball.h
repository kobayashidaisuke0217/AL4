#pragma once
#include "../../Engine/3d/worldTransform.h"
#include "ViewProjection.h"
#include"Sphere.h"

class Ball
{
public:
	void Init();
	void Update();
	void Draw(ViewProjection& view);
	static Ball* GetInstance();
	void Setparent(const WorldTransform* parent);
	void DeleteParent();
public:
	WorldTransform transform_;
	Sphere* sphere_;
	Vector3 velocity_;
	StructSphere structSphere_;
};

