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
	Quaternion q1 = { 1.0f,2.0f,3.0f,4.0f };
	Quaternion q2 = { 2.0f,1.0f,3.0f,5.0f };
    Vector4 identity;
    Vector4 conj;
    Vector4 inv;
    Vector4 normal;
    Vector4 mul1;
    Vector4 mul2;
	float norm;

};

