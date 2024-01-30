#pragma once
#include "Sprite.h"
#include "myMath.h"
#include <memory>
#include "textureManager.h"
#include "BlueMoon.h"
class Fade
{
public:
	static Fade* GetInstance();

	void Initialize();
	void Update();
	void SelectUpdate();
	void Draw();

	void setmoveFlag() { moveFlag_ = true; }
	void setSelectMoveFlag() { selectmoveFlag_ = true; }
	bool GetSelectMoveFlag() { return selectmoveFlag_; }
	bool getchangeFlag() { return ChangeFlag_; }
	float GetMaterialArufa() { return material.w; }
	void SetMaterialArufa(float arufa) { material.w = arufa; }
private:
	Texturemanager* texturManager_;
	bool moveFlag_;
	bool ChangeFlag_;
	bool arfaUp_;
	Vector3 move_;
	std::unique_ptr<Sprite> sprite_;
	uint32_t tex;
	void Move();
	BlueMoon* blueMoon_;
	Transform saikorotransform_;
	Transform 	SpriteuvTransform;
	Vector4 material;
	bool selectmoveFlag_;
	bool selectarfaUp_;

};