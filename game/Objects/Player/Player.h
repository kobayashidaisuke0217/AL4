#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"
#include "ViewProjection.h"
#include "Collider.h"
#include "CollisionConfig.h"
#include "Icharactor.h"
#include <optional>
#include"GlobalVariables.h"
class LookOn;
struct ConstAttack {
	//振りかぶり
	uint32_t anticipationTIme;
	//ため
	uint32_t chargeTime;
	//攻撃振り
	uint32_t swingTime;
	//硬直
	uint32_t recoveryTime;
	//振りかぶりの速度
	float anticipationSpeed;
	//ための速度
	float chargeSpeed;
	//攻撃振りの速度
	float swingSpeed;
};
static const float anticipationRotate = 3.642f;
static const float anticipationRotateHammer = 0.5f;

static const float swingRotate = 1.542f;
static const float swingRotateHammer = -1.6f;

class Player :public Collider, public ICharactor
{
	
public:
	
	void Initialize(const std::vector<Model*>& models,Vector3 pos) override;
	void Update()override;
	void Draw(const ViewProjection& view)override;
	WorldTransform GetWorldTransform()override { return worldTransformBody_; }
	const WorldTransform& GetWorldTransformBase() { return worldTransformBody_; }
	void SetViewProjection(const ViewProjection* view) { viewProjection_ = view; }
	void IsFall();
	StructSphere GetStructSphere() { return structSphere_; }
	bool isHit_;
	bool isGameover() { return gameOver; }
	void OnCollision() override;
	void Setparent(const WorldTransform* parent);
	void IsCollision(const WorldTransform& worldtransform);
	void DeleteParent();
	void SetObjectPos(const WorldTransform& worldtransform) { objectPos_ = worldtransform; }
	const WorldTransform& GetWorldTransformBody() { return worldTransformBody_; }
	OBB getcollsionObb() { return collisionObb_; }
	bool GetIsAtack() { return isAtack; }
	void SetLockOn( LookOn* lock) { LockOn_ = lock; }
	static const int comboNum = 3;
	static const std::array<ConstAttack, comboNum> kConstAttacks_;
private:
	enum class Behavior {
		kRoot,
		kAtack,
		kDash,
	};
	struct WorkDash {
		uint32_t dashParameter_ = 0;
		Vector3 move_;
		uint32_t cooltime_;
		uint32_t currentcooltime_;
		float velocity_;
	};
	struct WorkAtack {
		uint32_t Time;
		float rotate;
		float hammerRotate;
		int32_t comboIndex = 0;
		int32_t inComboPhase = 0;
		bool comboNext = false;
	};
	WorkDash workDash_;
	WorkAtack workAtack_;
	Vector4 color;
	Input* input_ = nullptr;
	const ViewProjection* viewProjection_ = nullptr;
	StructSphere structSphere_;
	bool gameOver = false;
	WorldTransform worldTransform_;

	WorldTransform worldTransformHead_;
	WorldTransform worldTransformLarm_;
	WorldTransform worldTransformRarm_;
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformHammer_;
	WorldTransform objectPos_;
	float floatingParametor_ = 0.0f;
	float animationFrame;

	Behavior behavior_ = Behavior::kRoot;
	std::optional<Behavior> behaviorRequest_ = std::nullopt;
	float cosin;
	OBB collisionObb_;
	Matrix4x4 Direction_;
	Quaternion quaternion_;
	Vector3 preMove_;
	Quaternion preQuaternion_;
	bool isDash_;
	bool isAtack;
	bool isMove_;
	float moveSpeed_;
	LookOn* LockOn_;
private:
	void Move();
	void SetParentModel(const WorldTransform* parent);
	void ModelUpdateMatrix();
	void InitializeFloatGimmick();
	void UpdateFloatGimmick();

	void BehaviorRootUpdate();
	void BehaviorAtackUpdate();
	void BehaviorRootInitialize();
	void BehaviorAtackInitialize();

	void ApplyGlobalVariables();

	void BehaviorDashInitialize();

	void BehaviorDashUpdate();
};