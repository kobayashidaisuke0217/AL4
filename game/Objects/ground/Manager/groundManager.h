#pragma once
#include "ground.h"
#include"MoveGround.h"
#include<memory>
#include "viewProjection.h"
#include "Model.h"
#include "myMath.h"
using namespace std;
class groundManager
{
public:
	void Initialize();
	void Update();
	void Draw(const ViewProjection& view);
	OBB GetOBB(int num) { return Obb_[num]; }
    MoveGround* GetMoveGround() { return moveGround_.get(); }
	ground* GetGround(int num) { return ground_->get(); }
private:
	
	unique_ptr<ground>ground_[2];
	unique_ptr<MoveGround> moveGround_;
	unique_ptr<Model> model_;
	unique_ptr<Model> movemodel_;
	OBB Obb_[3];
};

