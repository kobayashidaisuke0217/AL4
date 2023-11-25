#pragma once
#include "viewProjection.h"
#include "Sprite.h"
#include "Input.h"
#include "WorldTransform.h"
#include "Sphere.h"
class Enemy;
class LookOn
{
public:
	void Initialize();
	void Update(const std::list<Enemy*>& enemys, const ViewProjection& viewProjection);
	void Draw();
	void Target();
	void Search(const std::list<Enemy*>& enemys, const ViewProjection& viewProjection);
	void Reset();
	Vector3 GetTargetPos() const;
	bool isTarget();
	bool Existtarget()const { return target_ ? true : false; }
	bool isRangeOut(const ViewProjection& viewProjection);
private:
	XINPUT_STATE preInputPad;
	std::unique_ptr<Sprite> Mark_;
	 Enemy* target_ = nullptr;
	Transform SpriteTransform_;

	int texhandle_;
	float minDistance_ = 10.0f;
	float maxDistance_ = 30.0f;
	float angleRange_ = 50.0f * 3.14159265f / 180.0f;
	bool isLockOn_;
	int count_;
	bool isAut = true;
	std::list<std::pair<float, Enemy*>>targets;
	int iteratornum;
	int max = 0;
	Vector3 WorldToScreen(Vector3 world, const ViewProjection& viewProjection);
};

