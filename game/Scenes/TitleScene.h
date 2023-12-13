#pragma once
#include "Iscene.h"
#include "Input.h"
#include "myMath.h"
class TitleScene:public Iscene
{
private:
	int count;
public:
	~TitleScene()override;
	 void Initialize() override;

	 void Update() override;

	 void Draw() override;
	 void Finalize()override;
private:
	Input* input;
	Quaternion Rotation;
	Vector3 pointY = { 2.1f,-0.9f,1.3f };
	Matrix4x4 rotateMatrix;
	Vector3 rotateByQuaternion;
	Vector3 rotateByMatrix;

};

