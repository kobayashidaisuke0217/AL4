#include "groundManager.h"

void groundManager::Initialize()
{
	for (int i = 0; i < 2; i++) {
		ground_[i] = make_unique<ground>();
	
	}
	moveGround_ = make_unique<MoveGround>();
	movemodel_.reset(Model::CreateModelFromObj("Resource", "move.obj"));
	model_.reset(Model::CreateModelFromObj("Resource", "cube.obj"));
	model_->SetColor({ 0.0f,0.0f,0.0f,1.0f });
	int map[7][11] = {
		{1,1,1,1,1,1,1,1,1,1,1},
		{1,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,1},
	    {1,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,1},
	    {1,1,1,1,1,1,1,1,1,1,1},

	};
	index_ = 0;
	for (int i = 0; i < 11; i++) {
		for (int j = 0; j < 7; j++) {
			
			worldTransform_[index_].Initialize();
			map_[j][i] = map[j][i];
			worldTransform_[index_].translation_ = { /*-50.0f +*/ i* 8.0f ,-1.0f,/*20.0f -*/ j * 8.0f };
			worldTransform_[index_].scale_ = { 4.0f,1.0f,4.0f };
			index_++;
		}
	}
	//worldTransform_[0].translation_ = { 54.0f,-10.0f,30.0f };
	//worldTransform_[0].scale_ = { 54.0f,-10.0f,30.0f };

}

void groundManager::Update()
{

	for (int i = 0; i < 77; i++) {
		worldTransform_[i].UpdateMatrix();
	}
}

void groundManager::Draw(const ViewProjection& view)
{
	index_ = 0;
	for (int i = 0; i < 11; i++) {
		for (int j = 0; j < 7; j++) {
			if (map_[j][i] == 0) {
				movemodel_->Draw(worldTransform_[index_], view);
				
			}
			else {
				model_->Draw(worldTransform_[index_], view);
			}
			index_++;
		}
	}/*model_->Draw(worldTransform_[0], view);*/
	//moveGround_->Draw(view);
}
