#pragma once
#include "ground.h"
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
private:
	list<ground*> groundList_;
	unique_ptr<ground>ground_[2];
	unique_ptr<Model> model_;
	OBB Obb_[2];
};

