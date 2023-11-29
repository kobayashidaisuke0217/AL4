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
	Vector3 from0 = Normalise({ 1.0f,0.7f,0.5f });
	Vector3 to0 = -from0;
	Vector3 from1 = Normalise({ -0.6f,0.9f,0.2f });
	Vector3 to1 = Normalise({ 0.4f,0.7f,-0.5f });
};

