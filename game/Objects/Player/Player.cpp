#include "Player.h"
#include "ImguiManger.h"
void Player::Initialize( Model* model)
{
	worldTransform_.Initialize();
	input_ = Input::GetInstance();
	model_ = model;
	isHit_ = false;
	SetCollisionAttribute(CollisionConfig::kCollisionAttributePlayer);
	SetCollisionMask(~CollisionConfig::kCollisionAttributePlayer);
	color={ 1.0f,1.0f,1.0f,1.0f };
	
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
		Setparent(&worldTransform_);
		
		Vector3 worldPos = worldTransform_.GetWorldPos();
		Vector3 objectWorldPos = { worldtransform.matWorld_.m[3][0],worldtransform.matWorld_.m[3][1],worldtransform.matWorld_.m[3][2] };
		Vector3 Position = worldPos - objectWorldPos;

		Matrix4x4 rotatematrix = MakeRotateXYZMatrix({ -worldtransform.rotation_.x ,-worldtransform.rotation_.y ,-worldtransform.rotation_.z });
		Position = TransformNormal(Position, rotatematrix);
		worldTransform_.translation_ = Position;
		
	}
}

void Player::Move()
{
	XINPUT_STATE joystate;
	if (Input::GetInstance()->GetJoystickState(0, joystate)) {
		const float kCharctorSpeed = 0.3f;
		Vector3 move = {
			(float)joystate.Gamepad.sThumbLX / SHRT_MAX, 0.0f,
			(float)joystate.Gamepad.sThumbLY / SHRT_MAX };
		move = Multiply(kCharctorSpeed, Normalise(move));
		Matrix4x4 rotateMatrix = MakeRotateMatrix(viewProjection_->rotation_);
		move = TransformNormal(move, rotateMatrix);
		worldTransform_.translation_ = Add(move, worldTransform_.translation_);
		worldTransform_.rotation_.y = std::atan2(move.x, move.z);
	}
}
