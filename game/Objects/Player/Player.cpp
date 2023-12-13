#include "Player.h"

void Player::Initialize(const std::vector<Model*>& models, Vector3 pos)
{
	ICharactor::Initialize(models, pos);
	worldTransform_.translation_ = { 0.0f,2.0f,-20.0f };
	worldTransformBody_.translation_ = { 0.0f,2.0f,-20.0f };
	worldTransformHead_.translation_ = { 0.0f, 1.0f, 0.0f };
	worldTransformLarm_.translation_ = { -0.2f, 1.0f, 0.0f };
	worldTransformRarm_.translation_ = { 0.2f, 1.0f, 0.0f };
	worldTransform_.Initialize();
	worldTransformBody_.Initialize();
	worldTransformHead_.Initialize();
	worldTransformLarm_.Initialize();
	worldTransformRarm_.Initialize();
	worldTransformBase_.Initialize();
	SetParent(&GetWorldTransformBody());
	quaternion_ = createQuaternion(0.0f, { 0.0f,1.0f,0.0f });
	quaternion_ = Normalize(quaternion_);
	input_ = Input::GetInstance();
}

void Player::Update()
{
		structSphere_.center = worldTransformBody_.GetWorldPos();
		structSphere_.radius = 1.5f;
		UpdateFloatGimmick();

		if (isSelected) {
			MovePlay();
		}
		else {
			MoveAI();
		}

		ModelUpdateMatrix();

}





void Player::Draw(const ViewProjection& view)
{

	if (isAlive_ == true) {
		models_[kModelBody]->Draw(worldTransformBody_, view);
		models_[kModelHead]->Draw(worldTransformHead_, view);
		models_[kModelLarm]->Draw(worldTransformLarm_, view);
		models_[kModelRarm]->Draw(worldTransformRarm_, view);
	}
}


void Player::isCollision()
{


}

void Player::MoveAI()
{
	const float kCharctorSpeed = 0.3f;
	Vector3 move = {
		0.0f, 0.0f,
		1.0f };

	MoveVec = move;

	if (CompereVector3(move, { 0.0f,0.0f,0.0f })) {
		isMove_ = false;
	}
	else {
		isMove_ = true;
	}
	if (isMove_ == true) {
		/*Matrix4x4 rotateMatrix = MakeRotateMatrix(viewProjection_->rotation_);
		move = TransformNormal(move, rotateMatrix);*/

		move = Multiply(kCharctorSpeed, Normalise(move));

		worldTransform_.translation_ = Add(move, worldTransform_.translation_);

		preQuaternion_ = quaternion_;
		worldTransformBody_.translation_ = worldTransform_.translation_;
		//プレイヤーの行きたい方向
		Vector3 newPos = Subtract(Add(worldTransformBody_.translation_, move), worldTransformBody_.translation_);
		Vector3 Direction;
		//プレイヤーの現在の向き
		Direction = TransformNormal({ 1.0f,0.0f,0.0f }, quaternionToMatrix(quaternion_));

		Direction = Normalise(Direction);
		Vector3 newDirection = Normalise(newPos);
		float cosin = Dot(Direction, newDirection);

		//行きたい方向のQuaternionの作成
		Quaternion newquaternion_;

		newquaternion_ = createQuaternion(cosin, { 0.0f,1.0f,0.0f });

		//quaternionの合成
		quaternion_ = Normalize(quaternion_);
		newquaternion_ = Normalize(newquaternion_);

		quaternion_ = Multiply(quaternion_, newquaternion_);
		if (CompereQuaternion(quaternion_, preQuaternion_) && !CompereVector3(move, preMove_)) {
			cosin = -1.0f;
			quaternion_ = Multiply(quaternion_, createQuaternion(cosin, { 0.0f,1.0f,0.0f }));
			//preQuaternion_ = quaternion_;
		}

		preMove_ = move;

	}
	worldTransformBody_.quaternion_ = Slerp(0.3f, worldTransformBody_.quaternion_, quaternion_);

	worldTransform_.quaternion_ = worldTransformBody_.quaternion_;
}


void Player::MovePlay()
{
	XINPUT_STATE joystate;
	if (Input::GetInstance()->GetJoystickState(0, joystate)) {
		const float kCharctorSpeed = 0.3f;
		float jumpvelo = 0.0f;

		Vector3 move = {
			(float)joystate.Gamepad.sThumbLX / SHRT_MAX, 0.0f,
			(float)joystate.Gamepad.sThumbLY / SHRT_MAX };

		MoveVec = move;

		if (CompereVector3(move, { 0.0f,0.0f,0.0f })) {
			isMove_ = false;
		}
		else {
			isMove_ = true;
		}
		if (isMove_ == true) {
			/*Matrix4x4 rotateMatrix = MakeRotateMatrix(viewProjection_->rotation_);
			move = TransformNormal(move, rotateMatrix);*/

			move = Multiply(kCharctorSpeed, Normalise(move));

			worldTransform_.translation_ = Add(move, worldTransform_.translation_);

			preQuaternion_ = quaternion_;
			worldTransformBody_.translation_ = worldTransform_.translation_;
			//プレイヤーの行きたい方向
			Vector3 newPos = Subtract(Add(worldTransformBody_.translation_, move), worldTransformBody_.translation_);
			Vector3 Direction;
			//プレイヤーの現在の向き
			Direction = TransformNormal({ 1.0f,0.0f,0.0f }, quaternionToMatrix(quaternion_));

			Direction = Normalise(Direction);
			Vector3 newDirection = Normalise(newPos);
			float cosin = Dot(Direction, newDirection);

			//行きたい方向のQuaternionの作成
			Quaternion newquaternion_;

			newquaternion_ = createQuaternion(cosin, { 0.0f,1.0f,0.0f });

			//quaternionの合成
			quaternion_ = Normalize(quaternion_);
			newquaternion_ = Normalize(newquaternion_);

			quaternion_ = Multiply(quaternion_, newquaternion_);
			if (CompereQuaternion(quaternion_, preQuaternion_) && !CompereVector3(move, preMove_)) {
				cosin = -1.0f;
				quaternion_ = Multiply(quaternion_, createQuaternion(cosin, { 0.0f,1.0f,0.0f }));
				//preQuaternion_ = quaternion_;
			}

			preMove_ = move;

		}
		worldTransformBody_.quaternion_ = Slerp(0.3f, worldTransformBody_.quaternion_, quaternion_);

		worldTransform_.quaternion_ = worldTransformBody_.quaternion_;
	}
}

void Player::SetParent(const WorldTransform* parent)
{
	worldTransformBase_.parent_ = parent;
	worldTransformHead_.parent_ = parent;
	worldTransformRarm_.parent_ = parent;
	worldTransformLarm_.parent_ = parent;
	worldTransform_.parent_ = parent;
}

void Player::ModelUpdateMatrix()
{
	worldTransform_.UpdateQuaternionMatrix();
	worldTransformBody_.UpdateQuaternionMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformRarm_.UpdateMatrix();
	worldTransformLarm_.UpdateMatrix();

}

void Player::InitializeFloatGimmick()
{
	floatingParametor_ = 0.0f;
}

void Player::UpdateFloatGimmick()
{
	uint16_t T = 120;

	float step = 2.0f * (float)M_PI / T;
	float floatingAmplitude = 0.3f;

	floatingParametor_ += step;
	floatingParametor_ = (float)std::fmod(floatingParametor_, 2.0f * M_PI);



	worldTransformBody_.translation_.y = std::sin(floatingParametor_) * floatingAmplitude + 1.0f;

	worldTransformLarm_.rotation_.x = std::sin(floatingParametor_) * 0.75f;
	worldTransformRarm_.rotation_.x = std::sin(floatingParametor_) * 0.75f;
}

