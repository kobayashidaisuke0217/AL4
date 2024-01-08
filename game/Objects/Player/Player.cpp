#include "Player.h"

void Player::Initialize(const std::vector<Model*>& models, Vector3 pos,int position)
{
	ball_ = Ball::GetInstance();
	ICharactor::Initialize(models, pos,position);
	map_ = MoveMap::GetInstance();
	worldTransform_.translation_ = pos;
	worldTransformBody_.translation_ = pos;
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
	inputHandle_ = std::make_unique<InputHandler>();
	positionNo_ = position;
	if (positionNo_ == LeftBack || positionNo_ == RightBack) {
		AtackEnd_ = 55.0f;
		DefEnd_ = 13.0f;
	}
	if (positionNo_ == MidFirld) {
		AtackEnd_ = 60.0f;
		DefEnd_ = 20.0f;
	}
	if (positionNo_ == GoalKeeper) {
		AtackEnd_ = 10.0f;
		DefEnd_ = 9.0f;
	}
	if (positionNo_ == Fowerd) {
		AtackEnd_ = 75.0f;
		DefEnd_ = 40.0f;
	}
	//マップを代入
	for (int i = 0; i < 11; i++) {
		for (int j = 0; j < 7; j++) {
			if (positionNo_ == LeftBack) {
				map[j][i] = map_->AtackLB[j][i];
			}
			else if (positionNo_ == RightBack) {
				map[j][i] = map_->AtackRB[j][i];
			}
			else if (positionNo_ == MidFirld) {
				map[j][i] = map_->AtackMF[j][i];
			}
			else if (positionNo_ == Fowerd) {
				map[j][i] = map_->AtackFW[j][i];
			}
		}
	}
	behavior_ = Behavior::kAtack;

	if (positionNo_ == GoalKeeper) {
		behavior_ = Behavior::kAtack;
	}
	ModelUpdateMatrix();
	count_ = 0;
}

void Player::Update()
{
	obb_.center = worldTransformBody_.GetWorldPos() ;//worldTransformHammer_.GetWorldPos();

	GetOrientations(MakeRotateMatrix({ 0.0f,0.0f,0.0f }),obb_.orientation);
	obb_.size = { 1.5f,1.0f,1.5f };
	if (positionNo_ != GoalKeeper) {
	if (worldTransformBody_.translation_.x <= 7.0f || worldTransformBody_.translation_.x >= 80.0f) {
		behaviorRequest_ = Behavior::kReturn;
	}if (worldTransformBody_.translation_.z <= 7.0f || worldTransformBody_.translation_.z >= 48.0f) {
		behaviorRequest_ = Behavior::kReturn;
	}
	
		if (map[mapZ_][mapX_] == 0&&behavior_!=Behavior::kReturn) {
			behaviorRequest_ = Behavior::kAtack;
			prevMapX_ = currentMapX_;
			prevMapZ_ = currentMapZ_;

		}
	}
		structSphere_.center = worldTransformBody_.GetWorldPos();
		structSphere_.radius = 1.5f;
		UpdateFloatGimmick();
		Vector3 world = worldTransformBody_.GetWorldPos();
		
		currentMapX_ = mapX_;
		currentMapZ_ = mapZ_;
		mapX_ = (int)world.x / mapSize;
		mapZ_ = (int)world.z / mapSize;
		if (currentMapX_ !=nextmapX_ ) {
			prevMapX_ = currentMapX_;
			
		}
		if (currentMapZ_ != nextMapZ_) {
			prevMapZ_ = currentMapZ_;
		}
		if (isSelected) {
			 move_ = {
			(float)input_->joystate.Gamepad.sThumbLX / SHRT_MAX, 0.0f,
			(float)input_->joystate.Gamepad.sThumbLY / SHRT_MAX };
			 command = inputHandle_->PlayerHandleInput(this);
			 
		}
		else {
			//move_={1.0f,0.0f,0.0f};
			command = SelectAICommand();
		}
		
		command->Exec();
		ModelUpdateMatrix();
	
		if (IsCollision(obb_, ball_->structSphere_)&&passCount_>=20) {
			isBall_ = true;
			ball_->Setparent(&worldTransformBody_);
			ball_->velocity_ = { 0.0f,0.0f,0.0f };
		}
		else {
			//ball_->DeleteParent();
			isBall_ = false;
		}
		passCount_++;
		ImGui::Begin("player");
		ImGui::DragFloat3("tra", &world.x);
		ImGui::End();
		count_++;
		if (count_ >= 10) {
			count_ = 0;
		}
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




void Player::Move(Vector3 Move)
{
	
	
		const float kCharctorSpeed = 0.3f;
		float jumpvelo = 0.0f;

		Vector3 move = Move;

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
		if (positionNo_ != GoalKeeper) {
			worldTransformBody_.quaternion_ = Slerp(0.3f, worldTransformBody_.quaternion_, quaternion_);

			worldTransform_.quaternion_ = worldTransformBody_.quaternion_;
		}
	move_ = { 0.0f,0.0f,0.0f };
	
}

void Player::Pass()
{
	
	if (isBall_) {
		passCount_ = 0;
		ball_->DeleteParent();
		Vector3 vel =
		{
			(float)input_->joystate.Gamepad.sThumbLX / SHRT_MAX, 0.0f,
				(float)input_->joystate.Gamepad.sThumbLY / SHRT_MAX };

		ball_->velocity_ = Normalise(vel)*0.4f;
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
	/*uint16_t T = 120;

	float step = 2.0f * (float)M_PI / T;
	float floatingAmplitude = 0.3f;

	floatingParametor_ += step;
	floatingParametor_ = (float)std::fmod(floatingParametor_, 2.0f * M_PI);



	worldTransformBody_.translation_.y = std::sin(floatingParametor_) * floatingAmplitude + 1.0f;

	worldTransformLarm_.rotation_.x = std::sin(floatingParametor_) * 0.75f;
	worldTransformRarm_.rotation_.x = std::sin(floatingParametor_) * 0.75f;*/
}

ICommand* Player::SelectAICommand()
{
	if (behaviorRequest_) {
		behavior_ = behaviorRequest_.value();
		switch (behavior_) {
		case Behavior::kAtack:
			break;
		case Behavior::kBack:
			break;
		case Behavior::kWaitpass:
			break;
		default:
			break;
		}
		behaviorRequest_ = std::nullopt;
	}
	switch (behavior_) {
	case Behavior::kAtack:
		setMovevectorAtack();
		break;
	case Behavior::kBack:
		setMovevectorAtack();
		break;
	case Behavior::kWaitpass:
		setMoveVectorWaitPass();
		break;
	case Behavior::kReturn:
		setMoveVectorRerturn();
	default:
		break;
	}
	return new CharaMoveCommand(this);
}

void Player::setMovevectorAtack()
{
	
	bool isLoop = false;
	if (behavior_ == Behavior::kAtack) {
		for (int i = 0; i < 11; i++) {
			for (int j = 0; j < 7; j++) {
				if (map[j][i] == 45) {
					
						nextmapX_ = i;
					
					
						nextMapZ_ = j;
					
					isLoop = true;
					break;
				}

			}
			if (isLoop) {
				break;
			}
			
		}
		if (prevMapX_ == nextmapX_ && prevMapZ_ == nextMapZ_) {

		}
		if (!isLoop&& positionNo_ != GoalKeeper) {
			behaviorRequest_ = Behavior::kBack;
			return;
		}
		if (map[mapZ_][mapX_]>=44|| map[mapZ_][mapX_]==0 && positionNo_ != GoalKeeper) {
			behaviorRequest_ = Behavior::kBack;
			return;
		}
		//move_ = { 1.0f,0.0f,0.0f };
		velocity_ = Normalise(Vector3((float)nextmapX_ * 8.0f, 1.0f, (float)nextMapZ_ * 8.0f) - worldTransformBody_.GetWorldPos());
		velocity_.y = 0.0f;
		move_ = velocity_;
		if (positionNo_ == GoalKeeper) {
			move_ = { 1.0f,0.0f,0.0f };
		}
	
		if (worldTransformBody_.translation_.x >= AtackEnd_) {
		
			behaviorRequest_= Behavior::kBack;
		}
	}
	else {
		backCount_++;
		for (int i = 0; i < 11; i++) {
			for (int j = 0; j < 7; j++) {
				if (map[j][i] == map[mapZ_][mapX_]-1) {
					if (prevMapX_ != i) {
						nextmapX_ = i;
					}
					if (prevMapZ_ != j) {
						nextMapZ_ = j;
					}
					isLoop = true;
					break;
				}

			}
			if (isLoop) {
				break;
			}

		}
		velocity_ = Normalise(Vector3((float)nextmapX_ * 8.0f, 1.0f, (float)nextMapZ_ * 8.0f) - worldTransformBody_.GetWorldPos());
		velocity_.y = 0.0f;
		move_ = velocity_;
		
		if (worldTransformBody_.translation_.x <= DefEnd_) {
			backCount_ = 0;
			behaviorRequest_ = Behavior::kAtack;
		}
		if (backCount_ >= 120) {
	        backCount_ = 0;
			behaviorRequest_ = Behavior::kAtack;
		}
		if (positionNo_ == GoalKeeper) {
			move_ = { -1.0f,0.0f,0.0f };
		}
	}
}

void Player::setMoveVectorWaitPass()
{
	Serach();
	move_ = velocity_;
	if (prevMapX_ == nextmapX_ && prevMapZ_ == nextMapZ_) {
		behavior_ = Behavior::kAtack;
	}
}

void Player::Serach()
{
	if (count_ == 0) {
		int plusZ = 0;
		int minesZ = 0;
		int plusX = 0;
		int minesX = 0;
		if (mapZ_ > 0) {
			
			plusZ = map[mapZ_ - 1][mapX_];
			if (prevMapZ_ == mapZ_ - 1&&prevMapX_==mapX_) {
				plusZ = -1;
			}
		}
		if (mapZ_ < 6 ) {
			minesZ = map[mapZ_ + 1][mapX_];
			if (prevMapZ_ == mapZ_ + 1 && prevMapX_ == mapX_) {
				minesZ = -1;
			}
		}
		if (mapX_ < 10 ) {
			plusX = map[mapZ_][mapX_ + 1];
			if (prevMapX_ == mapX_ + 1 && prevMapZ_ == mapZ_) {
				plusX = -1;
		}
		}
			if (mapX_ > 0) {
				minesX = map[mapZ_][mapX_ - 1];
				if (prevMapX_ == mapX_ - 1 && prevMapZ_ == mapZ_) {
					minesX = -1;
				}
		}
			int x, z = 0;
			int numz, numx = 0;
			if (plusX > minesX) {
				x = 1;
				numx = plusX;

			}
			else {
				x = -1;
				numx = minesX;
			}
			if (plusZ > minesZ) {
				z = -1;
				numz = plusZ;
			}
			else {
				z = 1;
				numz = minesZ;
			}
			if (numx >= numz) {
				nextmapX_ = mapX_ + x;
				nextMapZ_ = mapZ_;

			}
			else {
				
				nextMapZ_ = mapZ_ + z;
				nextmapX_ = mapX_;

			}
			velocity_ = Normalise(Vector3((float)nextmapX_ * 8.0f, 1.0f, (float)nextMapZ_ * 8.0f) - worldTransformBody_.GetWorldPos());
			velocity_.y = 0.0f;
		}
	
	}

void Player::setMoveVectorRerturn()
{
	velocity_=  Normalise(Vector3(6.0f * 8.0f, 1.0f, 4.0f * 8.0f) - worldTransformBody_.GetWorldPos());
	velocity_.y = 0.0f;
	move_ = velocity_;
}


