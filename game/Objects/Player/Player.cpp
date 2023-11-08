#include "Player.h"
#include "ImguiManger.h"
void Player::Initialize(const std::vector<Model*>& models)
{
	ICharactor::Initialize(models);
	//worldTransformBody_.translation_ = { 0.0f,2.0f,50.0f };
	worldTransformHead_.translation_ = { 0.0f, 1.0f, 0.0f };
	worldTransformLarm_.translation_ = { -0.2f, 1.0f, 0.0f };
	worldTransformRarm_.translation_ = { 0.2f, 1.0f, 0.0f };
	worldTransform_.Initialize();
	worldTransformBody_.Initialize();
	worldTransformHead_.Initialize();
	worldTransformLarm_.Initialize();
	worldTransformRarm_.Initialize();
	worldTransformHammer_.Initialize();
	SetParentModel(&GetWorldTransformBody());
	input_ = Input::GetInstance();
	models_[kModelHead] = models[kModelHead];
	models_[kModelBody] = models[kModelBody];
	models_[kModelLarm] = models[kModelLarm];
	models_[kModelRarm] = models[kModelRarm];
	models_[kModelHammer] = models[kModelHammer];
	isHit_ = false;
	SetCollisionAttribute(CollisionConfig::kCollisionAttributePlayer);
	SetCollisionMask(~CollisionConfig::kCollisionAttributePlayer);
	color = { 1.0f,1.0f,1.0f,1.0f };
	worldTransform_.translation_ = { 1.0f,2.5f,1.0f };
	GlovalVariables* globalVariables{};
	globalVariables = GlovalVariables::GetInstance();
	quaternion_ = createQuaternion(0.0f, { 0.0f,1.0f,0.0f });
	quaternion_ = Normalize(quaternion_);
	const char* groupName = "Player";
	GlovalVariables::GetInstance()->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "HammerScale", worldTransformHammer_.scale_);
	globalVariables->AddItem(groupName, "HammerPos", worldTransformHammer_.translation_);
	globalVariables->AddItem(groupName, "DashSpeed", workDash_.velocity_);
	globalVariables->AddItem(groupName, "DashCoolTime",(float) workDash_.cooltime_);
	worldTransformHammer_.scale_ = globalVariables->GetVector3Value(groupName, "HammerScale");
	worldTransformHammer_.translation_ = globalVariables->GetVector3Value(groupName, "HammerPos");
	ApplyGlobalVariables();
	moveSpeed_ = 0.1f;
}

void Player::Update()
{
	ApplyGlobalVariables();
	XINPUT_STATE joyState;
	collisionObb_.center = worldTransformHammer_.GetWorldPos();
	//collisionObb_.center.y += 1.0f;
	GetOrientations(MakeRotateXYZMatrix(worldTransformHammer_.rotation_), collisionObb_.orientation);
	collisionObb_.size = { 1.0f,3.0f,1.0f };
	if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
		return;
	}
	if (worldTransform_.GetWorldPos().y < -10.0f) {
		gameOver = true;
	}



	structSphere_.center = worldTransformBody_.GetWorldPos();
	structSphere_.radius = 1.5f;

	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
		behaviorRequest_ = Behavior::kAtack;
	}
	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
		if (workDash_.cooltime_ <= workDash_.currentcooltime_) {
			behaviorRequest_ = Behavior::kDash;
			
		}

	}
	if (!isDash_) {
		workDash_.currentcooltime_++;
	}
	if (behaviorRequest_) {
		behavior_ = behaviorRequest_.value();
		switch (behavior_) {
		case Behavior::kRoot:
		default:
			BehaviorRootInitialize();
			break;
		case Behavior::kAtack:
			BehaviorAtackInitialize();
			break;
		case Behavior::kDash:
			BehaviorDashInitialize();

		}
		behaviorRequest_ = std::nullopt;
	}
	switch (behavior_) {
	case Behavior::kRoot:
	default:
		BehaviorRootUpdate();
		break;
	case Behavior::kAtack:

		BehaviorAtackUpdate();

		break;
	case Behavior::kDash:
		BehaviorDashUpdate();
		break;
	}

	Vector3 a = worldTransformBody_.GetWorldPos();
	Vector3 b = worldTransform_.GetWorldPos();
	
	ModelUpdateMatrix();


}

void Player::Draw(const ViewProjection& view)
{
	models_[kModelBody]->Draw(worldTransformBody_, view);
	models_[kModelHead]->Draw(worldTransformHead_, view);
	models_[kModelLarm]->Draw(worldTransformLarm_, view);
	models_[kModelRarm]->Draw(worldTransformRarm_, view);
	if (isAtack) {
		models_[kModelHammer]->Draw(worldTransformHammer_, view);
	}
}

void Player::IsFall()
{
	worldTransform_.translation_.y -= 0.1f;
	worldTransformBody_.translation_ = worldTransform_.GetWorldPos();
}

void Player::OnCollision()
{
	gameOver = true;
}

void Player::Setparent(const WorldTransform* parent)
{

	worldTransform_.parent_ = parent;
	worldTransformBody_.parent_ = parent;

}

void Player::IsCollision(const WorldTransform& worldtransform)
{
	if (!worldTransform_.parent_) {

		worldTransform_.translation_.y = worldtransform.translation_.y + worldtransform.scale_.y + worldTransform_.scale_.y;
		Vector3 worldPos = worldTransformBody_.GetWorldPos();
		Vector3 objectWorldPos = { worldtransform.matWorld_.m[3][0],worldtransform.matWorld_.m[3][1],worldtransform.matWorld_.m[3][2] };
		Vector3 Position = worldPos - objectWorldPos;

		Matrix4x4 rotatematrix = MakeRotateXYZMatrix({ -worldtransform.rotation_.x ,-worldtransform.rotation_.y ,-worldtransform.rotation_.z });
		Position = TransformNormal(Position, rotatematrix);

		worldTransform_.translation_ = Position;
		worldTransformBody_.translation_ = worldTransform_.translation_;
		Setparent(&worldtransform);
		gameOver = false;
	}
}

void Player::DeleteParent()
{
	if (worldTransform_.parent_) {
		worldTransform_.translation_ = worldTransformBody_.GetWorldPos();
		worldTransformBody_.translation_ = worldTransform_.GetWorldPos();
		worldTransform_.parent_ = nullptr;
		worldTransformBody_.parent_ = nullptr;
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
		if (CompereVector3(move, { 0.0f,0.0f,0.0f })) {
			isMove_ = false;
		}
		else {
			isMove_ = true;
		}
		if (isMove_ == true) {
			Matrix4x4 rotateMatrix = MakeRotateMatrix(viewProjection_->rotation_);
			move = TransformNormal(move, rotateMatrix);
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

		worldTransformBody_.quaternion_ = Slerp(0.3f,   worldTransformBody_.quaternion_, quaternion_);
		
		worldTransform_.quaternion_ = worldTransformBody_.quaternion_;
	}
	}
	

void Player::SetParentModel(const WorldTransform* parent)
{
	worldTransformHead_.parent_ = parent;
	worldTransformRarm_.parent_ = parent;
	worldTransformLarm_.parent_ = parent;
	worldTransform_.parent_ = parent;
	worldTransformHammer_.parent_ = parent;
}

void Player::ModelUpdateMatrix()
{
	worldTransform_.UpdateQuaternionMatrix();
	worldTransformBody_.UpdateQuaternionMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformRarm_.UpdateMatrix();
	worldTransformLarm_.UpdateMatrix();
	worldTransformHammer_.UpdateMatrix();
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


	if (!isHit_ || worldTransformBody_.GetWorldPos().y < 0.0f) {
		IsFall();
	}

	else {
		worldTransform_.translation_.y = objectPos_.translation_.y + objectPos_.scale_.y + worldTransform_.scale_.y;
		worldTransformBody_.translation_.y = std::sin(floatingParametor_) * floatingAmplitude + 1.0f;
	}
	worldTransformLarm_.rotation_.x = std::sin(floatingParametor_) * 0.75f;
	worldTransformRarm_.rotation_.x = std::sin(floatingParametor_) * 0.75f;
}
void Player::BehaviorRootUpdate() {
	Move();
	UpdateFloatGimmick();
}

void Player::BehaviorAtackUpdate() {
	if (animationFrame < 10) {
		worldTransformLarm_.rotation_.x += 0.05f;
		worldTransformRarm_.rotation_.x += 0.05f;

		worldTransformHammer_.rotation_.x += 0.05f;
	}
	else if (worldTransformHammer_.rotation_.x >= -2.0f * (float)M_PI / 4) {
		worldTransformLarm_.rotation_.x -= 0.1f;
		worldTransformRarm_.rotation_.x -= 0.1f;

		worldTransformHammer_.rotation_.x -= 0.1f;
	}
	else {
		behaviorRequest_ = Behavior::kRoot;
	}

	animationFrame++;

}

void Player::BehaviorRootInitialize() {
	worldTransformLarm_.rotation_.x = 0.0f;
	worldTransformRarm_.rotation_.x = 0.0f;
	worldTransformHammer_.rotation_.x = 0.0f;

	InitializeFloatGimmick();

	worldTransformBody_.Initialize();
	worldTransformHead_.Initialize();
	worldTransformLarm_.Initialize();
	worldTransformRarm_.Initialize();
	worldTransformHammer_.Initialize();
	isAtack = false;
	isDash_ = false;
	workDash_.currentcooltime_ = 0;
}

void Player::BehaviorAtackInitialize() {
	worldTransformLarm_.rotation_.x = (float)M_PI;
	worldTransformRarm_.rotation_.x = (float)M_PI;
	worldTransformHammer_.rotation_.x = 0.0f;
	animationFrame = 0;
	isAtack = true;
	isDash_ = false;
	workDash_.currentcooltime_ = 0;
}

void Player::ApplyGlobalVariables()
{
	GlovalVariables* globalVariables = GlovalVariables::GetInstance();

	const char* groupName = "Player";

	
	workDash_.velocity_ = globalVariables->GetFloatValue(groupName, "DashSpeed");
	workDash_.cooltime_= (uint32_t)globalVariables->GetFloatValue(groupName, "DashCoolTime");
}

void Player::BehaviorDashInitialize()
{
	workDash_.dashParameter_ = 0;
	isAtack = false;
}

void Player::BehaviorDashUpdate()
{
	const uint32_t behaviorDashTime = 30;
	XINPUT_STATE joystate;
	if (!isDash_) {
		isDash_ = true;
		workDash_.currentcooltime_ = 0;
		if (Input::GetInstance()->GetJoystickState(0, joystate)) {
			
			workDash_.move_ = {
				(float)joystate.Gamepad.sThumbLX / SHRT_MAX, 0.0f,
				(float)joystate.Gamepad.sThumbLY / SHRT_MAX };
			
			workDash_.move_ = Multiply(workDash_.velocity_  , Normalise(workDash_.move_));
			Matrix4x4 rotateMatrix = MakeRotateMatrix(viewProjection_->rotation_);
			workDash_.move_ = TransformNormal(workDash_.move_, rotateMatrix);
		}
	}
	else {
	   //移動方向に合わせてダッシュ
		
		
		worldTransform_.translation_ = Add(workDash_.move_, worldTransform_.translation_);
		worldTransformBody_.translation_ = worldTransform_.translation_;
		//プレイヤーの行きたい方向
		Vector3 newPos = Subtract(Add(worldTransformBody_.translation_, workDash_.move_), worldTransformBody_.translation_);
		Vector3 Direction;
		//プレイヤーの現在の向き
		Direction = TransformNormal({ 1.0f,0.0f,0.0f }, quaternionToMatrix(quaternion_));

		Direction = Normalise(Direction);
		Vector3 newDirection = Normalise(newPos);
		float cosin = Dot(Direction, newDirection)/**3.14159265f/2.0f*/;

		//行きたい方向のQuaternionの作成
		Quaternion newquaternion_;

		newquaternion_ = createQuaternion(cosin, { 0.0f,1.0f,0.0f });

		//quaternionの合成
		quaternion_ = Normalize(quaternion_);
		newquaternion_ = Normalize(newquaternion_);
		quaternion_ = Multiply(quaternion_, newquaternion_);

	}
	
	if (++workDash_.dashParameter_ >= behaviorDashTime) {
		behaviorRequest_ = Behavior::kRoot;
		isDash_ = false;
		
	}
}


