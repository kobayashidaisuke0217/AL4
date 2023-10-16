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
	goalRotate_ = { 0.0f,0.0f,0.0f };
	startRotate_ = { 0.0f,0.0f,0.0f };
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
	ImGui::DragFloat4("translation", &worldTransform_.rotation_.x,0.01f);
	ImGui::End();
	
	worldTransform_.UpdateMatrix();
	worldTransform_.scale_ = { 1.0f,1.0f,1.0f };
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
		quaternion_ = createQuaternion(rad, { 1.0f,0.0f,0.0f });
		Matrix4x4 quaternionMat = quaternionToMatrix(quaternion_);
		Matrix4x4 goalmatrix = Multiply(worldTransform_.matWorld_, quaternionMat);

		Vector3 Rotate = matrixToEulerAngles(goalmatrix);//quaternionToEulerAngles(quaternion_);
		goalRotate_ = worldTransform_.rotation_+Rotate;
		startRotate_ = worldTransform_.rotation_;
		MoveFlag = true;
	}
	if (input_->PushKey(DIK_S) && MoveFlag == false) {
		quaternion_ = createQuaternion(rad, { -1.0f,0.0f,0.0f });
		Matrix4x4 quaternionMat = quaternionToMatrix(quaternion_);
		Matrix4x4 goalmatrix = Multiply(worldTransform_.matWorld_, quaternionMat);

		Vector3 Rotate = matrixToEulerAngles(goalmatrix);//quaternionToEulerAngles(quaternion_);
		goalRotate_ = worldTransform_.rotation_ + Rotate;
		startRotate_ = worldTransform_.rotation_;
		MoveFlag = true;
	}
	if (input_->PushKey(DIK_A) && MoveFlag == false) {
		quaternion_ = createQuaternion(rad, { 0.0f,0.0f,1.0f });
		Matrix4x4 quaternionMat = quaternionToMatrix(quaternion_);
		Matrix4x4 goalmatrix = Multiply(worldTransform_.matWorld_, quaternionMat);

		Vector3 Rotate = matrixToEulerAngles(goalmatrix);//quaternionToEulerAngles(quaternion_);
		goalRotate_ = worldTransform_.rotation_ + Rotate;
		startRotate_ = worldTransform_.rotation_;
		MoveFlag = true;
	}
	if (input_->PushKey(DIK_D) && MoveFlag == false) {
	
		quaternion_ = createQuaternion(rad, { 0.0f,0.0f,-1.0f });
		Matrix4x4 quaternionMat = quaternionToMatrix(quaternion_);
		Matrix4x4 goalmatrix = Multiply(worldTransform_.matWorld_, quaternionMat);

		Vector3 Rotate = matrixToEulerAngles(goalmatrix);//quaternionToEulerAngles(quaternion_);
		goalRotate_ = worldTransform_.rotation_ + Rotate;
		startRotate_ = worldTransform_.rotation_;
		MoveFlag = true;
	}
	if (input_->PushKey(DIK_SPACE)) {
		worldTransform_.rotation_.x += 1.58f;
	}
	if (input_->PushKey(DIK_RETURN)) {
		worldTransform_.rotation_.x -= 1.58f;
	}
	/*if (worldTransform_.rotation_.y >= 6.0f || worldTransform_.rotation_.y <= -6.0f) {
		worldTransform_.rotation_.y = 0.0f;
	}
	if (worldTransform_.rotation_.z >= 6.0f || worldTransform_.rotation_.z <= -6.0f) {
		worldTransform_.rotation_.z = 0.0f;
	}
	if (worldTransform_.rotation_.x >= 6.0f || worldTransform_.rotation_.x <= -6.0f) {
		worldTransform_.rotation_.x = 0.0f;
	}*/

	
	if (MoveFlag == true) {
		if (moveSpeed <= 1.0f) {
			moveSpeed += 0.05f;
		}else
		 {
			moveSpeed = 1.0f;
		}
		worldTransform_.rotation_ = Lerp(moveSpeed, startRotate_, goalRotate_);
		if (moveSpeed >= 1.0f) {
			MoveFlag = false;
			moveSpeed = 0.0f;
		}
	}
}
