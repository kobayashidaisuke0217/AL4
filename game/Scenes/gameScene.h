#pragma once
#include "BlueMoon.h"
#include "Input.h"
#include "MyMath.h"
#include "triangle.h"
#include "Iscene.h"
#include "Sprite.h"
#include "Sphere.h"
#include "model.h"
#include "worldTransform.h"
#include "ViewProjection.h"
#include "CollisionManager.h"
#include "Texturemanager.h"
#include "GlobalVariables.h"
#include "SkyDome.h"
#include"FollowCamera.h"
#include "groundManager.h"
#include "Enemy.h"
#include <list>
#include "Engine/2D/particle.h"
#include "game/Objects/Player/Player.h"
#include "../Manager/PlayerManager.h"
#include "../Manager/MoveMap.h"
#include"../Manager/Ball.h"
#include "../Objects/Goal.h"
using namespace std;
class GameScene :public Iscene
{
public:
	~GameScene();
	void Initialize()override;
	void Update()override;

	void Draw()override;
	void Finalize()override;
private:
#pragma region 基本機能
	BlueMoon* blueMoon_;
	DirectXCommon* directXCommon_;
	Sprite* sprite_;
	Texturemanager* textureManager_;
	Input* input_;
	ViewProjection viewProjection_;
	int blendCount_;
	void Draw2D();
	void Draw3D();
	void ApplyGlobalVariables();
#pragma endregion
#pragma region ゲームの機能
	int a;
	unique_ptr<Particle> particle;
	int count_;
	unique_ptr<SkyDome> skyDome_ = nullptr;
	unique_ptr<Model> skyDomeModel_ = nullptr;
	unique_ptr<Model> playerModel_ = nullptr;
	unique_ptr<FollowCamera> followCamera_;
	//unique_ptr<ground> ground_;
	unique_ptr<groundManager> groundmanager_;
	unique_ptr<CollisionManager> collisionManager_;
	list<Enemy*> enemys_;
	unique_ptr<Enemy>enemy_;
	unique_ptr<PlayerManager> player_;
	std::unique_ptr<Model> enemyHeadModel = nullptr;
	std::unique_ptr<Model> enemyBodyModel = nullptr;
	std::unique_ptr<Model> enemyL_armModel = nullptr;
	std::unique_ptr<Model> enemyR_armModel = nullptr;
	std::unique_ptr<Model> HeadModel = nullptr;
	std::unique_ptr<Model> BodyModel = nullptr;
	std::unique_ptr<Model> L_armModel = nullptr;
	std::unique_ptr<Model> R_armModel = nullptr;
	std::unique_ptr<Model> player_Hammer_ = nullptr;
	std::vector<Model*>enemyModels;
	std::unique_ptr<Goal>goal_;
	Ball* ball_ = nullptr;
	MoveMap* map_=nullptr;
	bool isGoal_;
	int goalCount_;
#pragma endregion
};