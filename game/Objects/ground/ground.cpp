#include "ground.h"

void ground::Initialize(Model*model,Vector3 translation, Vector3 Scale)
{
	texturemanager_ = Texturemanager::GetInstance();
	worldTtansform_.Initialize();
	worldTtansform_.translation_ = translation;
	worldTtansform_.scale_ = Scale;
	texhandle_ = texturemanager_->Load("Resource/wi.png");
	groundModel_ = model;
	groundModel_->SetColor({ 0.4f,1.0f,0.0f,1.0f });
}
void ground::Update()
{
	
	worldTtansform_.UpdateMatrix();
}

void ground::Draw(const ViewProjection& view)
{
	groundModel_->Draw(worldTtansform_, view);
}
