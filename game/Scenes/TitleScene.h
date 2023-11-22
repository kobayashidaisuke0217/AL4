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
	Input* input=nullptr;
	Vector3 axis=Normalise({1.0f,1.0f,1.0f});
	float angle = 0.44f;
	
};

