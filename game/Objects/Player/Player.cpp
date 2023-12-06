#include "Player.h"
#include "ImguiManger.h"
#include "Game/Manager/LookOn.h"
const std::array<ConstAttack, Player::comboNum>
Player::kConstAttacks_ = { {
	{5,1,20,10,0.0f,0.0f,0.15f},
	{15,10,15,10,0.2f,0.0f,0.0f},
	{15,10,10,30,0.2f,0.0f,0.0f},
	}
};
void Player::Initialize(const std::vector<Model*>& models,Vector3 pos)
{
	ICharactor::Initialize(models,pos);
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
	BehaviorRootInitialize();
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
	velo = 0;
}

void Player::Update()
{
	
	
	
	ApplyGlobalVariables();
	XINPUT_STATE joyState;
	Vector3 offset = { 0.0f,4.0f,0.0f };
	collisionObb_.center = worldTransformBody_.GetWorldPos() + (Normalise( QuatToEuler(worldTransformBody_.quaternion_)));//worldTransformHammer_.GetWorldPos();

	GetOrientations(MakeRotateMatrix({0.0f,0.0f,0.0f}), collisionObb_.orientation);
	collisionObb_.size = {3.0f,0.0f,3.0f };
	worldTransformHead_.translation_=collisionObb_.center;
	if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
		return;
	}
	if (!isHit_ || worldTransformBody_.GetWorldPos().y < 0.0f) {
		if (!isJump_) {
			IsFall();
		}

	}if (worldTransformBody_.translation_.y < 2.3f) {
		isJump_ = false;
	}


	if (worldTransform_.GetWorldPos().y < -10.0f) {
		gameOver = true;
	}
	
	structSphere_.center = worldTransformBody_.GetWorldPos();
	structSphere_.radius = 1.5f;

	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B) {
		if (!isAtack) {
			behaviorRequest_ = Behavior::kAtack;
		}
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
	if (!isJump_) {
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_X) {
			if (!(prejoy.Gamepad.wButtons & XINPUT_GAMEPAD_X)) {
				isJump_ = true;
				velo = 1.5f;
				worldTransformBody_.translation_.y += 2.0f;
				worldTransform_.translation_ = worldTransformBody_.translation_;
				jumpCount = 0;
			}
		}
	}
	else {
		velo -= 0.05f;
		jumpCount++;
		
			worldTransformBody_.translation_.y += velo;
			worldTransform_.translation_.y = worldTransformBody_.translation_.y;
		
	}
	ModelUpdateMatrix();

	prejoy = joyState;
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
	if (velo >= 0.0f) {
		velo -= 0.1f;
	}
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
		float jumpvelo=0.0f;
		
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
		else if(LockOn_&&LockOn_->Existtarget()) {
			Vector3 Direction;
			//プレイヤーの現在の向き
			Direction = TransformNormal({ 1.0f,0.0f,0.0f }, quaternionToMatrix(quaternion_));

			Direction = Normalise(Direction);
			Vector3 newPos = Subtract( LockOn_->GetTargetPos(), worldTransformBody_.translation_);
			Vector3 newDirection = Normalise(newPos);
			float cosin = Dot(Direction, newDirection);

			//行きたい方向のQuaternionの作成
			Quaternion newquaternion_;

			newquaternion_ = createQuaternion(cosin, { 0.0f,1.0f,0.0f });

			//quaternionの合成
			quaternion_ = Normalize(quaternion_);
			newquaternion_ = Normalize(newquaternion_);

			quaternion_ = Multiply(quaternion_, newquaternion_);
		}

		worldTransformBody_.quaternion_ = Slerp(0.3f,   worldTransformBody_.quaternion_, quaternion_);
		
		worldTransform_.quaternion_ = worldTransformBody_.quaternion_;
	}
	}
	

void Player::SetParentModel(const WorldTransform* parent)
{
	//worldTransformHead_.parent_ = parent;
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


	
  if(isHit_) {
		worldTransform_.translation_.y = objectPos_.translation_.y + objectPos_.scale_.y + worldTransform_.scale_.y;
		worldTransformBody_.translation_.y = std::sin(floatingParametor_) * floatingAmplitude + 1.0f;
	}
	worldTransformLarm_.rotation_.x = std::sin(floatingParametor_) * 0.75f;
	worldTransformRarm_.rotation_.x = std::sin(floatingParametor_) * 0.75f;
}
void Player::BehaviorRootUpdate() {
	Move();
	//UpdateFloatGimmick();
}

void Player::BehaviorAtackUpdate() {
	XINPUT_STATE joyState;
	uint32_t anticipationTime = kConstAttacks_[workAtack_.comboIndex].anticipationTIme;
	uint32_t chargeTime = kConstAttacks_[workAtack_.comboIndex].anticipationTIme + kConstAttacks_[workAtack_.comboIndex].chargeTime;
	uint32_t swingTime = chargeTime + kConstAttacks_[workAtack_.comboIndex].swingTime;
	uint32_t recoveryTime = swingTime + kConstAttacks_[workAtack_.comboIndex].recoveryTime;
	if (workAtack_.comboIndex < comboNum - 1)
	{
		if (Input::GetInstance()->GetJoystickState(0, joyState)) {

			if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B)
			{
				if (workAtack_.Time >= 20) {
					workAtack_.comboNext = true;
				}
				
			}
		}
	}
	if (workAtack_.Time >=recoveryTime )
	{
		if (workAtack_.comboNext)
		{
			workAtack_.comboNext = false;
			workAtack_.Time = 0;
			workAtack_.comboIndex++;

			

			switch (workAtack_.comboIndex)
			{
			case 0:
				workAtack_.rotate = (float)M_PI;
				workAtack_.hammerRotate = 0.0f;
				workAtack_.Time = 0;
				animationFrame = 0.0f;
				workAtack_.isAtack = false;
				break;

			case 1:
				workAtack_.rotate = (float)M_PI;
				workAtack_.hammerRotate = 0.0f;
				workAtack_.Time = 0;
				animationFrame = 0.0f;
				workAtack_.isAtack = false;
				break;

			case 2:
				workAtack_.rotate = (float)M_PI;
				workAtack_.hammerRotate = 0.0f;
				workAtack_.Time = 0;
				animationFrame = 0.0f;
				workAtack_.isAtack = false;
				break;
			}
		
		}
		else {
			behaviorRequest_ = Behavior::kRoot;
			
		}
	}
	
	if (workAtack_.Time < anticipationTime) {

		worldTransformLarm_.rotation_.x = Lerp(animationFrame, workAtack_.rotate , anticipationRotate);
		worldTransformRarm_.rotation_.x = Lerp(animationFrame, workAtack_.rotate, anticipationRotate);

		worldTransformHammer_.rotation_.x = Lerp(animationFrame, workAtack_.hammerRotate, anticipationRotateHammer);
		animationFrame += 1.0f / kConstAttacks_[workAtack_.comboIndex].anticipationTIme;
	}
	else if (workAtack_.Time < chargeTime) {
		workAtack_.rotate = worldTransformLarm_.rotation_.x;
		workAtack_.hammerRotate = worldTransformHammer_.rotation_.x;
		animationFrame = 0.0f;
	}
	else if (workAtack_.Time < swingTime) {

		worldTransformLarm_.rotation_.x = Lerp(animationFrame, workAtack_.rotate, swingRotate);
		worldTransformRarm_.rotation_.x = Lerp(animationFrame, workAtack_.rotate, swingRotate);

		worldTransformHammer_.rotation_.x = Lerp(animationFrame, workAtack_.hammerRotate, swingRotateHammer);
		animationFrame += 1.0f / kConstAttacks_[workAtack_.comboIndex].swingTime;
	}
	
	else if(workAtack_.Time>recoveryTime&&!workAtack_.comboNext) {
		behaviorRequest_ = Behavior::kRoot;
	}
	ImGui::Begin("atack");
	ImGui::DragFloat3("Larm", &worldTransformLarm_.rotation_.x);
	ImGui::DragFloat3("Rarm", &worldTransformRarm_.rotation_.x);
	ImGui::DragFloat3("Hammmer", &worldTransformHammer_.rotation_.x);
	ImGui::End();
	workAtack_.Time++;
	if (LockOn_ && LockOn_->Existtarget()) {
		Vector3 Direction;
		//プレイヤーの現在の向き
		Direction = TransformNormal({ 1.0f,0.0f,0.0f }, quaternionToMatrix(quaternion_));

		Direction = Normalise(Direction);
		Vector3 newPos = Subtract(LockOn_->GetTargetPos(), worldTransformBody_.translation_);
		Vector3 newDirection = Normalise(newPos);
		float cosin = Dot(Direction, newDirection);

		//行きたい方向のQuaternionの作成
		Quaternion newquaternion_;

		newquaternion_ = createQuaternion(cosin, { 0.0f,1.0f,0.0f });

		//quaternionの合成
		quaternion_ = Normalize(quaternion_);
		newquaternion_ = Normalize(newquaternion_);

		quaternion_ = Multiply(quaternion_, newquaternion_);
		worldTransformBody_.quaternion_ = Slerp(0.3f, worldTransformBody_.quaternion_, quaternion_);

	   worldTransform_.quaternion_ = worldTransformBody_.quaternion_;
	}

	
	

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
	workAtack_.hitAtack = 0;
	workAtack_.Combo = false;
}

void Player::BehaviorAtackInitialize() {
	workAtack_.rotate = (float)M_PI;
	workAtack_.hammerRotate = 0.0f;
	workAtack_.Time = 0;
	workAtack_.comboIndex = 0;
	animationFrame = 0.0f;
	isAtack = true;
	isDash_ = false;
	workDash_.currentcooltime_ = 0;
	workAtack_.hitAtack = 0;
	workAtack_.Combo = true;
	workAtack_.isAtack = false;
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


