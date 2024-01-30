#pragma once
#include "Iscene.h"
#include "Input.h"
#include "Sprite.h"
#include "../../game/Manager/Fade.h"
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
	std::unique_ptr<Sprite> sprite_;
	BlueMoon* bluemoon_;
	Input* input=nullptr;
	Fade* fade_;
};

