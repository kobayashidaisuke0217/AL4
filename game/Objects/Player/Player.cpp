#include "Player.h"
#include "ImguiManger.h"
#include "mymath.h"
void Player::Initialize( Model* model)
{
	worldTransform_.Initialize();
	input_ = Input::GetInstance();
	model_ = model;
	isHit_ = false;
	SetCollisionAttribute(CollisionConfig::kCollisionAttributePlayer);
	SetCollisionMask(~CollisionConfig::kCollisionAttributePlayer);
	color={ 1.0f,1.0f,1.0f,1.0f };
	moveSpeed = 0.0f;
	goal_= worldTransform_.matWorld_;
	start_ = worldTransform_.matWorld_;
	Translation_ = worldTransform_.translation_;
}

void Player::Update()
{
	if (worldTransform_.translation_.y < -10.0f) {
		gameOver = true;
	}
	if (!isHit_ || worldTransform_.translation_.y < -0.1f) {
		IsFall();
	}
	
	model_->SetColor(color);
	structSphere_.center = worldTransform_.GetWorldPos();
	structSphere_.radius = 1.5f;
	Move();
	ImGui::Begin("player");
	ImGui::DragFloat4("translation", &Translation_.x,0.01f);
	ImGui::End();
	//worldTransform_.UpdateMatrix();

	
}

void Player::Draw(const ViewProjection& view)
{
	model_->Draw(worldTransform_, view);
}

void Player::IsFall()
{
	worldTransform_.translation_.y -= 0.1f;
}

void Player::OnCollision()
{
	gameOver = true;
}

void Player::Setparent(const WorldTransform* parent)
{
	
		worldTransform_.parent_ = parent;
	
	
}

void Player::IsCollision(const WorldTransform& worldtransform)
{
	if (!worldTransform_.parent_) {
		worldTransform_.translation_.y = worldtransform.translation_.y;
		worldTransform_.matWorld_.m[3][0] -= worldtransform.matWorld_.m[3][0];
		worldTransform_.matWorld_.m[3][1] -= worldtransform.matWorld_.m[3][1];
		worldTransform_.matWorld_.m[3][2] -= worldtransform.matWorld_.m[3][2];

		Setparent(&worldTransform_);
	}
}

void Player::Move()
{
	
	if (input_->PushKey(DIK_W)&&MoveFlag==false) {
	    Translation_.z += 1.0f;
		quaternion_ = createQuaternion(rad, { -1.0f,0.0f,0.0f });
		quaternion_ = Normalize(quaternion_);
		Matrix4x4 quaternionMat = quaternionToMatrix(quaternion_);
		Matrix4x4 goalmatrix = Multiply(worldTransform_.matWorld_, quaternionMat);
		//goalmatrix = Multiply(goalmatrix, MakeTranslateMatrix(Translation_));
		
		goal_ = goalmatrix;
		start_ = worldTransform_.matWorld_;
		MoveFlag = true;
		
	}
	if (input_->PushKey(DIK_S) && MoveFlag == false) {
		worldTransform_.translation_.z -= 1.0f;
		quaternion_ = createQuaternion(rad, { 1.0f,0.0f,0.0f });
		quaternion_ = Normalize(quaternion_);
		Matrix4x4 quaternionMat = quaternionToMatrix(quaternion_);
		Matrix4x4 goalmatrix = Multiply(worldTransform_.matWorld_, quaternionMat);

		goal_ = goalmatrix;
		start_ = worldTransform_.matWorld_;
		MoveFlag = true;
	}
	if (input_->PushKey(DIK_A) && MoveFlag == false) {
		worldTransform_.translation_.x -= 1.0f;
		worldTransform_.translation_.y = 1.0f;
		quaternion_ = createQuaternion(rad, { 0.0f,0.0f,-1.0f });
		quaternion_ = Normalize(quaternion_);
		Matrix4x4 quaternionMat = quaternionToMatrix(quaternion_);
		Matrix4x4 goalmatrix = Multiply(worldTransform_.matWorld_, quaternionMat);
		
		goal_ = goalmatrix;
		start_ = worldTransform_.matWorld_;
		MoveFlag = true;
	}
	if (input_->PushKey(DIK_D) && MoveFlag == false) {
		worldTransform_.translation_.x += 1.0f;
		worldTransform_.translation_.y = 1.0f;
		quaternion_ = createQuaternion(rad, { 0.0f,0.0f,1.0f });
		quaternion_ = Normalize(quaternion_);
		Matrix4x4 quaternionMat = quaternionToMatrix(quaternion_);
		Matrix4x4 goalmatrix = Multiply(worldTransform_.matWorld_, quaternionMat);
		
	
		goal_ = goalmatrix;
		start_ = worldTransform_.matWorld_;
		MoveFlag = true;
	}
	

	
	if (MoveFlag == true) {
		if (moveSpeed <= 1.0f) {
			moveSpeed += 0.05f;
		}else
		 {
			moveSpeed = 1.0f;
		}
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				worldTransform_.matWorld_.m[i][j] = Lerp(moveSpeed, start_.m[i][j], goal_.m[i][j]);
			}
		}if (moveSpeed >= 1.0f) {
			MoveFlag = false;
			moveSpeed = 0.0f;
		}
		worldTransform_.TransferMatrix();
	}
}
