#pragma once
#include "Collider.h"
#include "CollisionConfig.h"
#include "WorldTransform.h"
#include "Model.h"
#include"Input.h"
#include"ViewProjection.h"
#include"ICharactor.h"
#include "game/Manager/ICommand.h"
#include "../../Manager/InputHandler.h"
#include <optional>
#include "../../Manager/MoveMap.h"
#include"../../Manager/Ball.h"
enum position {
	LeftBack,
	RightBack,
	MidFirld,
	Fowerd,
	GoalKeeper,
};
class Player:public ICharactor
{
public:
	void Initialize(const std::vector<Model*>& models, Vector3 pos,int positionNo) override;


	void Update()override;

	void Draw(const ViewProjection& view)override;
	
	StructSphere GetStructSphere() { return structSphere_; }
	void isCollision();
	const WorldTransform& GetWorldTransformBody() { return worldTransformBody_; }
	WorldTransform GetWorldTransform() { return worldTransformBase_; }
	bool GetisAlive() { return isAlive_; }
	void SetIsSelected(bool Flag) { isSelected = Flag; }
	OBB obb_;
	bool isBall_;
private:
	Ball* ball_;
	MoveMap* map_;
	enum class Behavior{
	  kAtack,
	  kBack,
	  kWaitpass,
	  kReturn
	};
	WorldTransform worldTransformBase_;
	WorldTransform worldTransformHead_;
	WorldTransform worldTransformLarm_;
	WorldTransform worldTransformRarm_;
	WorldTransform worldTransformBody_;
	Input* input_ = nullptr;
	const ViewProjection* viewProjection_ = nullptr;
	float floatingParametor_ = 0.0f;
	int animationFrame;
	//Vector3 move_;
	StructSphere structSphere_;
	bool isAlive_ = true;
	Vector4 color;
	bool isMove_;
	float moveSpeed_;
	Quaternion quaternion_;
	Vector3 preMove_;
	Quaternion preQuaternion_;
	Vector3 MoveVec;
	bool isSelected;
	ICommand* command;
	std::unique_ptr <InputHandler> inputHandle_;
	std::optional<Behavior>behaviorRequest_ = std::nullopt;
	Behavior behavior_ = Behavior::kAtack;
	int positionNo_;
	float AtackEnd_;
	float DefEnd_;
	int mapX_;
	int mapZ_;
	int nextmapX_;
	int nextMapZ_;
	int mapSize = 8;
	int map[7][11];
	int count_;
	int prevMapX_;
	int prevMapZ_;
	int currentMapX_;
	int currentMapZ_;
	Vector3 velocity_;
	
	int passCount_;
	int backCount_;
private:
	void MoveAI();
	void Move(Vector3 Move)override;
	void Pass()override;
	void SetParent(const WorldTransform* parent);
	void ModelUpdateMatrix();
	void InitializeFloatGimmick();
	void UpdateFloatGimmick();
	ICommand* SelectAICommand();
	void setMovevectorAtack();
	void setMoveVectorWaitPass();
	void Serach();
	void setMoveVectorRerturn();
};

