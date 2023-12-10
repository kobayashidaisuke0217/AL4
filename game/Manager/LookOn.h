#pragma once
#include "viewProjection.h"
#include "Sprite.h"
#include "Input.h"
#include "WorldTransform.h"
#include "Sphere.h"
#include <numbers>
#include "Enemy.h"
namespace AngleA {
	constexpr float Dig2Rad = std::numbers::pi_v<float> / 180.f;
}


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
	void AutLock(const std::list<Enemy*>& enemys, const ViewProjection& viewProjection);
	void SelectLock(const std::list<Enemy*>& enemys, const ViewProjection& viewProjection);
	bool Existtarget()const { return target_ ? true : false; }
	bool isRangeOut(const ViewProjection& viewProjection);
private:
	XINPUT_STATE preInputPad;
	std::unique_ptr<Sprite> Mark_;
	Enemy* target_ = nullptr;
	Transform SpriteTransform_;
	XINPUT_STATE joystate;
	int texhandle_;
	float minDistance_ = 2.0f;
	float maxDistance_ = 30.0f;
	float angleRange_ = 50.0f * AngleA::Dig2Rad;
	bool isLockOn_;
	int count_;
	bool isAut = true;
	std::vector<std::pair<float, Enemy*>>targets;
	int targetIndex;
	int max = 0;
	int cooltime = 0;
	Vector3 WorldToScreen(Vector3 world, const ViewProjection& viewProjection);
};

